/*
 * InterfaceAdapter.cpp
 *
 *  Created on: 09/11/2010
 *      Author: code
 */

#include "InterfaceAdapter.h"

int InterfaceAdapter::run() {
	return captureLoop(&capture_callback, (u_char *)this);
}

bool InterfaceAdapter::open(bool wait) {
	char buffer_data[32];
	if(Interface::open()) {
		sprintf(buffer_data, "not ether src %s", getMacAddressStr());
		compileFilter(buffer_data);
		setFilter();
		if(wait)
			Thread::startAndWait();
		else
			Thread::start();
		return true;
	}
	return false;
}

int InterfaceAdapter::callback_out(const char *data, int size) {
	return inject((const u_char *)data, size);
}

void InterfaceAdapter::capture_callback(u_char *user, const struct pcap_pkthdr* packet_header, const u_char* packet_data) {
	((InterfaceAdapter *)user)->capture(packet_header, packet_data);
}

void InterfaceAdapter::capture(const struct pcap_pkthdr* packet_header, const u_char* packet_data) {
	//EthPacket eth_packet(packet_data);
	//DEBUG("USB send, SRC: %s, DST: %s, size: %i\n", eth_packet.getSrcMACstr(), eth_packet.getDstMACstr(), packet_header->len);
	adp_in->callback_out((const char *)packet_data, packet_header->len);
}

void InterfaceAdapter::close() {
	breakLoop();
	wait();
	Interface::close();
	adp_in = NULL;
}

InterfaceAdapter::~InterfaceAdapter() {

}
