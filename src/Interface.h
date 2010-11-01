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

#ifndef INTERFACE_H_
#define INTERFACE_H_

#ifdef _WIN32
#define HAVE_REMOTE
#define WINVER 0x0501 //Windows XP
#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#endif
#include <pcap.h>
#include <string.h>
#include "InterfaceInfo.h"
#include "List.h"
#include "Logger.h"

class Interface {
private:
	char *dev;
	pcap_t *handle;
	pcap_pkthdr* packet_header;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	unsigned char mac[6];
	char mac_str[18];
	static void delete_info(void *);
public:
	Interface(const char *dev);
	bool open();
	void close();
	const unsigned char *getMacAddress();
	const char *getMacAddressStr();
	int setdirection(pcap_direction_t d = PCAP_D_IN);
	int captureLoop(pcap_handler packet_func, u_char *data);
	int capture(const void *packet_data, size_t size);
	int inject(const u_char *packet_data, size_t size);
	int compileFilter(char *filter);
	static List *getAdapterList();
	//int updateFilters(DeviceContainer *cont);
	const char *getLastError();
	const char *geterr();
	int setFilter();
	void breakLoop();
	virtual ~Interface();
};

#endif /*INTERFACE_H_*/
