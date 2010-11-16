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

#include <stdio.h>
#include "Socket.h"

#ifdef _WIN32
bool Socket::init = false;
#endif

Socket::Socket(int sock, sockaddr_in *addr) {
	this->sock = sock;
	memcpy(&client, addr, sizeof(client));
	port = 0;
}

Socket::Socket(const char *addr, int port) {
	sock = 0;
	strcpy(host, addr);
	this->port = port;
}

bool Socket::connectSocket(socket_type proto) {
#ifdef _WIN32
	if(!WSAStart())
		return false;
#endif
	struct addrinfo hints, *res;
	char port_s[5];
	sprintf(port_s,"%d",port);
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = proto;
	hints.ai_family = AF_INET;
	int error = getaddrinfo(host, port_s, &hints, &res);
	if(!error) {
		if((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) >= 0) {
#ifdef _WIN32
			int iOptVal = 2;
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iOptVal, sizeof(timeval));
#else
			struct timeval tv;
			tv.tv_sec = 2;
			tv.tv_usec = 0;
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(timeval));
#endif
			error = connect(sock, res->ai_addr, res->ai_addrlen);
			freeaddrinfo(res);
			return error >= 0;
		}
	}
	return false;
}

Socket::~Socket() {

}
