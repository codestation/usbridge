/*
 * SocketAdapter.h
 *
 *  Created on: 10/11/2010
 *      Author: code
 */

#ifndef SOCKETADAPTER_H_
#define SOCKETADAPTER_H_

#include "Socket.h"
#include "Adapter.h"
#include "Thread.h"

class SocketAdapter: private Socket, public Adapter, private Thread {
	//called by another adapter
	int callback_out(const char *data, int size);
	// called by adapter
	bool open(bool wait);
	//internal thread
	int run();
	bool loop;
public:
	SocketAdapter(const char *addr, int port): Socket(addr, port), loop(true) {}
	void close();
	virtual ~SocketAdapter();
};

#endif /* SOCKETADAPTER_H_ */
