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

#include "ServerSocket.h"

ServerSocket::ServerSocket(int port)  {
	sock = 0;
	this->port = port;
}

bool ServerSocket::bindSocket(socket_type proto) {
#ifdef _WIN32
	if(!WSAStart())
		return false;
#endif
	if((sock = socket(PF_INET, proto, 0)) < 0)
		return false;
#ifdef _WIN32
	int iOptVal = 2;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iOptVal, sizeof(timeval));
#else
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(timeval));
#endif
	sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	return bind( sock, (struct sockaddr *)&server, sizeof(server)) == 0;
}

bool ServerSocket::listenConnection(int max) {
	return listen( sock, max) != -1;
}

Socket * ServerSocket::acceptConnection() {
	sockaddr_in data;
	Socket *newsock = NULL;
	socklen_t len = sizeof(data);
	int res = accept( sock, (sockaddr *)&data, &len);
	if(res >= 0)
		newsock = new Socket(res, &data);
	return newsock;
}
/*
bool ServerSocket::readAvailable() {
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	if(select(-1, &readfds, NULL, NULL, &timeout) < 0)
		return false;
	else
		return FD_ISSET(sock, &readfds);
}
*/

ServerSocket::~ServerSocket() {
}
