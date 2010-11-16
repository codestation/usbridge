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

#ifndef SOCKET_H_
#define SOCKET_H_

#ifdef _WIN32
#define WINVER 0x0501 //Windows XP
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include "AbstractSocket.h"

#define PACKET_HEADER 10

class Socket: public AbstractSocket {
private:
	char host[16];
	sockaddr_in client;
#ifdef _WIN32
	static bool init;
	LPVOID lpMsgBuf;
#endif

public:

	Socket(const char *addr, int port);
	Socket(int s, sockaddr_in *);
	bool connectSocket(socket_type proto);

	inline const char *getIpAddress() { return inet_ntoa(client.sin_addr); };
	inline int getPort() { return client.sin_port; }

	virtual ~Socket();
};

#endif /*SOCKET_H_*/
