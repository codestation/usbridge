/*
 * UsbCommAdapter.cpp
 *
 *  Created on: 16/11/2010
 *      Author: code
 */

#include <string.h>
#include "Logger.h"
#include "UsbCommAdapter.h"

static const char magic[] = { 0x12, 0x08, 0x2F, 0x78 };

bool UsbCommAdapter::startHandshake() {
	unsigned char buffer[32];
	int actual = 0;
	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, magic, sizeof(magic));
	int r = bulk_write(buffer, 4, &actual, 1000);
	if(r) {
		ERR(">>Send SYN to device failed\n");
		return false;
	} else {
		INFO(">>SYN sent\n");
	}
	r = bulk_read(buffer, sizeof(buffer), &actual, 1000);
	if(r) {
		ERR(">>Failed to receive ACK\n");
		return false;
	} else {
		INFO(">>Received ACK\n");
		if(!memcmp(buffer, magic, sizeof(magic)) && actual > 11) {
			INFO(">>Sending SYN ACK\n");
			bulk_write(buffer, 12, &actual, 1000);
			return true;
		}
	}
	return false;
}

bool UsbCommAdapter::open(bool wait) {
	if(startHandshake()) {
		set_endpoints(129, 3);
		if(wait)
			Thread::startAndWait();
		else
			Thread::start();
		return true;
	}
	return false;
}
int UsbCommAdapter::run() {
	char buffer_data[2048];
	unsigned char tmp_buffer[512];
	char *buffer_offset = buffer_data;
	int actual;
	unsigned int total = 0;
	int incomplete = 0;
	while(loop) {
		if(!incomplete) {
			total = 0;
		}
		int res = bulk_read(tmp_buffer, 512, &actual, 1000);
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
		adp_in->callback_out(buffer_data, total);
	}
	return 0;
}

int UsbCommAdapter::callback_out(const char *data, int size) {
	unsigned char buffer[2048];
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
	memcpy(buffer + 0x18, data, size);
	int actual = 0;
	//FIXME: max packet length
	int res = bulk_write(buffer, size + 0x18, &actual, 1000);
	if(res == LIBUSB_ERROR_TIMEOUT) {
		DEBUG("USB timeout\n");
	} else if(res) {
		ERR("USB bulk OUT failed, code %i\n", res);
	} else {
		DEBUG("USB bulk OUT success, wrote %i bytes\n", actual);
		return actual;
	}
	return res;
}

void UsbCommAdapter::close() {
	loop = false;
	Thread::wait();
	UsbComm::release();
	UsbComm::reset();
	UsbComm::close();
	adp_in = NULL;
}

UsbCommAdapter::~UsbCommAdapter() {
	// TODO Auto-generated destructor stub
}
