/*
 * UsbCommAdapter.h
 *
 *  Created on: 16/11/2010
 *      Author: code
 */

#ifndef USBCOMMADAPTER_H_
#define USBCOMMADAPTER_H_

#include "UsbComm.h"
#include "Adapter.h"
#include "Thread.h"

class UsbCommAdapter: public UsbComm, public Adapter, private Thread {
	//called by another adapter
	int callback_out(const char *data, int size);
	// called by adapter
	bool open(bool wait);
	//internal thread
	int run();
	bool loop;
	bool startHandshake();
public:
	UsbCommAdapter(): UsbComm(), loop(true) {}
	void close();
	virtual ~UsbCommAdapter();
};

#endif /* USBCOMMADAPTER_H_ */
