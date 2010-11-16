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

#ifndef STREAMSOCKET_H_
#define STREAMSOCKET_H_

#include <stdio.h>
#include "Socket.h"

class StreamSocket {
	Socket *sock;
	FILE *stream;
public:
	StreamSocket(Socket *s);
	bool openStream();
	char *readStream(char *data, int size);
	bool writeStream(const char *data);
	void closeStream();
	virtual ~StreamSocket();
};

#endif /* STREAMSOCKET_H_ */
