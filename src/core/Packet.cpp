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

#include "Packet.h"

Packet::Packet() {
	packet = new packet_container;
	packet->header.head = HEADER_MAGIC;
	packet->header.id = 0;
	packet->header.counter = 0;
	packet->header.size = sizeof(packet->data);
}

void Packet::setPayload(const u_char *data, size_t size) {
	memcpy(&packet->data, data, size);
	packet->header.size = size;
}
void Packet::setPayload(EthPacket *pkt, size_t size) {
	memcpy(&packet->data, pkt->data(), size);
	packet->header.size = size;
}

Packet::~Packet() {
	delete packet;
}
