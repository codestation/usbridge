/*
 *  Project Bahamut: full ad-hoc tunneling software to be used by the
 *  Playstation Portable (PSP) to emulate online features.
 *
 *  Copyright (C) 2008-2010  Codestation
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Logger.h"
#include "DeviceBridge.h"

DeviceBridge::DeviceBridge() {
	capture_enabled = true;
	cap = NULL;
	inj = NULL;
	eth = NULL;
	usb = NULL;
	remote_mac = NULL;
	unregister = false;
	remote_mac = new List(compareFunc, deleteFunc);
}

bool DeviceBridge::makeBridge(const char *dev, UsbComm *usb) {
	eth = new Interface(dev);
	// doesnt return until connection end
	bool res = makeBridge(eth, usb);
	delete eth;
	return res;
}
bool DeviceBridge::makeBridge(Interface *eth, UsbComm *usb) {
	this->usb = usb;
	if(startHandshake()) {
		//FIXME: remove this fixed thing
		usb->set_endpoints(129, 3);
	if(eth->open()) {
		INFO("Start of inject_thread. Starting thread\n");
		char buffer_data[128];
		this->start();
		sprintf(buffer_data, "not ether src %s", eth->getMacAddressStr());
		eth->compileFilter(buffer_data);
		eth->setFilter();
		INFO("Filter applied: %s\n", buffer_data);
		//FIXME: disable setdirection in USB mode
		//eth->setdirection();
		// starts loop, doesnt return until close
		INFO("Start of capture_callback. Starting loop\n");
		eth->captureLoop(capture_callback, (u_char *)this);
		INFO("End of capture_callback. Finished loop\n");
		this->stop();
		this->wait();
		eth->close();
		return true;
	}
	}
	return false;
}

void DeviceBridge::capture_callback(u_char *user, const struct pcap_pkthdr* packet_header, const u_char* packet_data) {
	((DeviceBridge *)user)->capture(packet_header, packet_data);
}

void DeviceBridge::capture(const struct pcap_pkthdr* packet_header, const u_char* packet_data) {
	unsigned char buffer[2048];
	if(!capture_enabled)
		return;
	if(packet_header->len <= MAX_PAYLOAD_SIZE) {
		if(cap && cap(packet_data, packet_header->len))
			return;
		EthPacket eth_packet(packet_data);
		/////////////
		if(!remote_mac->exist((void *)eth_packet.getSrcMAC())) {
			remote_mac->add(new DeviceInfo(eth_packet.getSrcMAC(), remote_mac->count()));
			INFO("Registered new remote MAC: %s\n", eth_packet.getSrcMACstr());
		}
		/////////////
		DEBUG("USB send, SRC: %s, DST: %s, size: %i\n", eth_packet.getSrcMACstr(), eth_packet.getDstMACstr(), packet_header->len);
		memset(buffer, 0, 0x18);
		buffer[0] = 0x13;
		buffer[1] = 0x08;
		buffer[2] = 0x2F;
		buffer[3] = 0x78;
		buffer[4] = 0x04;
		buffer[8] = 0xef;
		buffer[9] = 0xcc;
		buffer[10] = 0x9a;
		buffer[11] = 0x90;
		buffer[12] = 0x03;
		memcpy(buffer + 0x18, packet_data, packet_header->len);
		int actual = 0;
		//FIXME: max packet length
		int res = usb->bulk_write(buffer, packet_header->len + 0x18, &actual, 1000);
		if(res == LIBUSB_ERROR_TIMEOUT) {
			DEBUG("USB timeout\n");
		} else if(res) {
			ERR("USB bulk OUT failed, code %i\n", res);
		} else {
			DEBUG("USB bulk OUT success, wrote %i bytes\n", actual);
		}
	}
}

int DeviceBridge::run() {
	unsigned char buffer_data[2048];
	unsigned char tmp_buffer[512];
	unsigned char *buffer_offset = buffer_data;
	int actual;
	unsigned int total;
	int incomplete = 0;
	while(capture_enabled) {
		if(!incomplete) {
			total = 0;
		}
		int res = usb->bulk_read(tmp_buffer, 512, &actual, 1000);
		if(res == LIBUSB_ERROR_TIMEOUT) {
			DEBUG("USB timeout\n");
			continue;
		} else if(res) {
			ERR("USB bulk IN failed, code %i\n", res);
			break;
		}
		DEBUG("USB bulk IN success, read %i bytes\n", actual);
		total += !total ? actual - 0x18 : actual - 0x8;
		if(total > sizeof(buffer_data)) {
			ERR("Packet size excceded buffer, discarding\n");
			incomplete = 0;
			buffer_offset = buffer_data;
			continue;
		}
		if(actual == 512) {
			memcpy(buffer_offset, !incomplete ? tmp_buffer + 0x18 : tmp_buffer + 0x8, !incomplete ? actual - 0x18 : actual - 0x8);
			buffer_offset += !incomplete ? actual - 0x18 : actual - 0x8;
			incomplete++;
			continue;
		} else {
			incomplete = false;
			memcpy(buffer_offset, !incomplete ? tmp_buffer + 0x18 : tmp_buffer + 0x8, !incomplete ? actual - 0x18 : actual - 0x8);
			buffer_offset = buffer_data;
		}
		EthPacket eth_packet(buffer_data);
		//eth_packet.hexdump();
		////////////////////////
		if(!eth_packet.isBroadcast()) {
			if(remote_mac->empty()) {
				DEBUG("Remote list empty. Discarding\n");
				continue;
			}
			if(!remote_mac->exist((void *)eth_packet.getDstMAC())) {
				INFO("Captured packet with unknown destination. Discarding\n");
				continue;
			}
		}
		//////////////

		if(inj && inj(&eth_packet))
			continue;
		DEBUG("Injecting packet SRC: %s, DST: %s, size: %i bytes\n", eth_packet.getSrcMACstr(), eth_packet.getDstMACstr(), total);
		eth->inject(buffer_data, total);
	}
	INFO("End of inject_thread. Finished thread\n");
	return 0;
}

bool DeviceBridge::startHandshake() {
	unsigned char buffer[32];
	int actual = 0;
	memset(buffer, 0, sizeof(buffer));
	buffer[0] = 0x12;
	buffer[1] = 0x08;
	buffer[2] = 0x2F;
	buffer[3] = 0x78;
	int r = usb->bulk_write(buffer, 4, &actual, 1000);
	if(r) {
		ERR(">>Send SYN to device failed\n");
		return false;
	} else {
		INFO(">>SYN sent\n");
	}
	r = usb->bulk_read(buffer, sizeof(buffer), &actual, 1000);
	if(r) {
		ERR(">>Failed to receive ACK\n");
		return false;
	} else {
		INFO(">>Received ACK\n");
		if(*(int *)buffer == 0x782F0812 && actual > 11) {
			INFO(">>Sending SYN ACK\n");
			usb->bulk_write(buffer, 12, &actual, 1000);
			return true;
		}
	}
	return false;
}

void DeviceBridge::removeBridge() {
	unregister = true;
	//speed->stop();
	this->stop();
	this->wait();
	eth->breakLoop();
}
void DeviceBridge::registerCaptureCallback(CAPTURE_FUNC func) {
	cap = func;
}
void DeviceBridge::registerInjectCallback(INJECT_FUNC func) {
	inj = func;
}

const char *DeviceBridge::getLastError() {
	return 0;
}

int DeviceBridge::compareFunc(void *obj, void *item) {
	return ((DeviceInfo *)obj)->compareMAC((u_char *)item);
}

void DeviceBridge::deleteFunc(void *obj) {
	delete (DeviceInfo *)obj;
}

void DeviceBridge::stop() {
	capture_enabled = false;
}

DeviceBridge::~DeviceBridge() {
	delete remote_mac;
}
