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

#include "AbstractSocket.h"

#ifdef _WIN32
bool AbstractSocket::init = false;
#endif

AbstractSocket::AbstractSocket() {
	sock = 0;
#ifdef _WIN32
	lpMsgBuf = 0;
#endif
}

#ifdef _WIN32
/*
 * Socket initializer (must be called before any socket function)
 * Parameters:
 * 		none
 * Returns: true if you can use the socket function, false otherwise
*/
bool AbstractSocket::WSAStart() {
	if(!init) {
		WSADATA info;
		if(!WSAStartup(MAKEWORD(2,2), &info))
			init = true;
	}
	return init;
}

/*
 * Socket cleaner (must be called when finish to use the socket functions)
 * Note: must be called for every time that WSAStartup was called
 * Parameters:
 * 		none
 * Returns: void
*/
void AbstractSocket::WSAClean() {
	if(init) {
		WSACleanup();
		init = false;
	}
}
#endif

ssize_t AbstractSocket::receiveData(char *buffer, size_t size) {
	return recv(sock, buffer, size, 0);
}

ssize_t AbstractSocket::receiveData(char *buffer, size_t size, sockaddr *from, socklen_t from_size) {
	return recvfrom(sock, buffer, size, 0, from, &from_size);
}

ssize_t AbstractSocket::sendData(const char *data, size_t length) {
	return send(sock, data, length , 0);
}

ssize_t AbstractSocket::sendData(const char *data, size_t length, sockaddr *to, socklen_t to_size) {
	return sendto( sock, data, length, 0, to, to_size);
}

void AbstractSocket::closeSocket() {
	if(sock) {
#ifdef _WIN32
		closesocket(sock);
		WSAClean();
#else
		close(sock);
#endif
		sock = 0;
	}
}

AbstractSocket::~AbstractSocket() {
	//closeSocket();
#ifdef _WIN32
	if(lpMsgBuf)
		LocalFree(lpMsgBuf);
#endif
}
