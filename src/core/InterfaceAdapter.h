/*
 * InterfaceAdapter.h
 *
 *  Created on: 09/11/2010
 *      Author: code
 */

#ifndef INTERFACEADAPTER_H_
#define INTERFACEADAPTER_H_

#include "Interface.h"
#include "Adapter.h"
#include "Thread.h"

class InterfaceAdapter: private Interface, public Adapter, private Thread {
	//called by another adapter
	int callback_out(const char *data, int size);
	// called by adapter
	bool open(bool wait);
	//internal thread
	int run();
	// called by captureLoop
	static void capture_callback(u_char* user, const struct pcap_pkthdr* packet_header, const u_char* packet_data);
	// called by capture_callback
	void capture(const struct pcap_pkthdr* packet_header, const u_char* packet_data);
public:
	InterfaceAdapter(const char *dev): Interface(dev) {}
	void close();
	virtual ~InterfaceAdapter();
};

#endif /* INTERFACEADAPTER_H_ */
