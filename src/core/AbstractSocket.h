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

#ifndef ABSTRACTSOCKET_H_
#define ABSTRACTSOCKET_H_

#ifdef _WIN32
#define WINVER 0x0501 //Windows XP
#include <windows.h>
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <sys/types.h>
#include <string.h>

class AbstractSocket {

#ifdef _WIN32
	LPVOID lpMsgBuf;
	static bool init;
#endif

protected:
	int sock;
	int port;
#ifdef _WIN32
	typedef int socklen_t;
#endif

public:
	enum socket_type {TCP_SOCKET = SOCK_STREAM, UDP_SOCKET = SOCK_DGRAM};

#ifdef _WIN32
protected:
	bool WSAStart();
	void WSAClean();
#endif

public:
	AbstractSocket();
	ssize_t receiveData(char *buffer, size_t size);
	ssize_t receiveData(char *buffer, size_t size,  sockaddr *from, socklen_t from_size);
	ssize_t sendData(const char *data, size_t length);
	ssize_t sendData(const char *buffer, size_t size,  sockaddr *to, socklen_t to_size);
	int getPort() { return port; }
	void closeSocket();
	virtual ~AbstractSocket();
	inline int getDescriptor() { return sock; }
	inline void shutdownSocket() {
#ifdef _WIN32
		shutdown(sock, SD_BOTH);
#else
		shutdown(sock, SHUT_RDWR);
#endif
	}

	inline const char *getLastErrorMessage() {
#ifdef _WIN32
		if(lpMsgBuf)
			LocalFree(lpMsgBuf);
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				WSAGetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0,
				NULL);
		return (const char *)lpMsgBuf;
#else
		return strerror(errno);
#endif
	}

	inline bool readAgain() {
#ifdef _WIN32
		return WSAGetLastError() == WSAEWOULDBLOCK || WSAETIMEDOUT;
#else
		return errno == EAGAIN;
#endif
	}

	inline int getLastError() {
#ifdef _WIN32
		return WSAGetLastError();
#else
		return errno;
#endif
	}

	inline void ssleep(int seconds) {
#ifdef _WIN32
		Sleep(1000 * seconds);
#else
		sleep(seconds);
#endif
	}
};

#endif /* ABSTRACTSOCKET_H_ */
