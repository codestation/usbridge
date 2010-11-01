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

#ifndef ETHPACKET_H_
#define ETHPACKET_H_

#ifdef _WIN32
#define WINVER 0x0501 //Windows XP
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_MTU_SIZE 1500

class EthPacket {
private:
	#pragma pack(push,1)
	struct eth_header {
		u_char dst_mac[6];
		u_char src_mac[6];
		u_char type[2];
	};
	#pragma pack(pop)

	#pragma pack(push,1)
	struct eth_data {
		eth_header header;
		u_char data[PACKET_MTU_SIZE - sizeof(eth_header)];
	};
	#pragma pack(pop)
	struct eth_data *eth;
	char src_mac_str[18];
	char dst_mac_str[18];
	static const u_char broadcast_mac[];

public:
	EthPacket(const u_char *packet_data);
	const u_char *getSrcMAC();
	const char *getSrcMACstr();
	const u_char *getDstMAC();
	const char *getDstMACstr();
	inline u_char *data() { return (u_char *)eth; }
	void setSrcMAC(const u_char *mac);
	void setDstMAC(const u_char *mac);
	bool isBroadcast();
	void hexdump();
	virtual ~EthPacket();
};

#endif /* ETHPACKET_H_ */
