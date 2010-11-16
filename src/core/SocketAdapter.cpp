/*
 * SocketAdapter.cpp
 *
 *  Created on: 10/11/2010
 *      Author: code
 */

#include "SocketAdapter.h"
#include "Logger.h"

bool SocketAdapter::open(bool wait) {
	if(Socket::connectSocket(Socket::UDP_SOCKET)) {
		if(wait)
			Thread::startAndWait();
		else
			Thread::start();
		return true;
	}
	return false;
}

int SocketAdapter::run() {
	char buffer[1024];
	while(loop) {
		int size = Socket::receiveData(buffer, sizeof(buffer));
		if(size > 0)
			adp_in->callback_out(buffer, size);
		else if(size < 0) {
			if(Socket::getLastError() == 0) {
				INFO("== TCPThread: End of stream reached\n");
				break;
			}
			if(!Socket::readAgain()) {
				ERR("error occurred while receiving packet: %s\n", Socket::getLastErrorMessage());
			}
		}
	}
	Socket::shutdownSocket();
	return 0;
}

int SocketAdapter::callback_out(const char *data, int size) {
	return Socket::sendData(data, size);
}

void SocketAdapter::close() {
	loop = false;
	Thread::wait();
	Socket::closeSocket();
	adp_in = NULL;
}

SocketAdapter::~SocketAdapter() {
}
