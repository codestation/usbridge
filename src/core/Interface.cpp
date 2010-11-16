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

#include "Interface.h"

/*
 * Default constructor
 * Parameters:
 * 		dev: name of the network device to open
 * Returns: void
*/
Interface::Interface(const char *dev) {
	handle = NULL;
	memset(&fp, 0, sizeof(fp));
	this->dev = strdup(dev);
}
/*
 * Default destructor
 * Parameters:
 * 		none
 * Returns: void
*/
Interface::~Interface() {
	//make sure that we close the interface before destroy the object
	close();
	free(dev);
}

/*
 * Tries to open the given interface on promiscuous
 * Parameters:
 * 		none
 * Returns: true if the interface is open, false otherwise
*/
bool Interface::open() {
#ifdef _WIN32
	return (handle = pcap_open(dev, BUFSIZ, PCAP_OPENFLAG_PROMISCUOUS | PCAP_OPENFLAG_NOCAPTURE_LOCAL | PCAP_OPENFLAG_MAX_RESPONSIVENESS, 500, NULL, errbuf)) != NULL;
#else
	return (handle = pcap_open_live(dev, BUFSIZ, 1, 500, errbuf)) != NULL;
#endif
}

int Interface::setdirection(pcap_direction_t d) {
	return pcap_setdirection(handle, d);
}

const char *Interface::geterr() {
	return pcap_geterr(handle);
}

const unsigned char *Interface::getMacAddress() {
#ifdef _WIN32
	char *realdev = 0;
	//search for the GUID (pcap returns the GUID with the "\Device\NPF_" prefix)
	if(!dev || !(realdev = strchr(dev, '{')))
		return NULL;
	PIP_ADAPTER_ADDRESSES pAddress, pAddresses = NULL;
	ULONG outBufLen = 0;

	if(GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &outBufLen) != ERROR_BUFFER_OVERFLOW)
		return NULL;
	pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);
	if(GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &outBufLen) != NO_ERROR)
		return NULL;
	pAddress = pAddresses;
	while (pAddress) {
		INFO("Adapter: %s\n", pAddress->AdapterName);
		if(!strcmp(pAddress->AdapterName, realdev)) {
			memcpy(mac, pAddress->PhysicalAddress, 6);
			break;
		}
		pAddress = pAddress->Next;
	}
	free(pAddresses);
	if(!pAddress)
		return NULL;
#else
	if(dev) {
		struct ifreq ifr;
		int fd = socket(AF_INET, SOCK_DGRAM, 0);
		ifr.ifr_addr.sa_family = AF_INET;
		strncpy(ifr.ifr_name, dev, IFNAMSIZ-1);
		if(ioctl(fd, SIOCGIFHWADDR, &ifr) == -1)
			return NULL;
		memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
	}
#endif
	return mac;
}

/*
 * Register a function callback and enter in a loop (this function doesnt
 * come back until breakLoop is called from another thread)
 * Parameters:
 * 		packet_func: a pointer of a function specified by pcap_handler
 * Returns: -1 if an error occurs, -2 if breakLoop was called
*/
int Interface::captureLoop(pcap_handler packet_func, u_char *data) {
	return pcap_loop ( handle, -1, packet_func, data);
}

/*
 * Puts the specified packet in the interface
 * Parameters:
 * 		packet_data: a pointer to the packet data
 * 		size: size of data to read
 * Returns: 0 if the packet was sent, -1 on error
*/
int Interface::inject(const u_char *packet_data, size_t size) {
	return pcap_sendpacket (handle, packet_data, size);
}

/*
 * Breaks the current captureLoop
 * Parameters:
		none
 * Returns: void
*/
void Interface::breakLoop() {
	pcap_breakloop(handle);
}

/*
 * Compiles a filter to be used at interface level (see pcap documentation)
 * 		If the result of the filter is true, the packet is captured, else
 * 		the packet is dropped.
 * Parameters:
		filter: filter sentence
 * Returns: 0 on success, -1 on erroneous filtering string
*/
int Interface::compileFilter(char *filter) {
	bpf_u_int32 pcap_netmask;
	bpf_u_int32 pcap_ip;
	if(pcap_lookupnet(dev, &pcap_ip, &pcap_netmask, errbuf) == -1)
		return -1;
	return pcap_compile(handle, &fp, filter, 1, pcap_netmask);
}

const char *Interface::getMacAddressStr() {
	unsigned const char *mac = getMacAddress();
	sprintf(mac_str,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return mac_str;
}

/*
 * Uses the current compiled filter on the interface
 * Parameters:
		none
 * Returns: 0 on success, -1 on error
*/
int Interface::setFilter() {
	int res = pcap_setfilter(handle, &fp);
	pcap_freecode(&fp);
	return res;
}

List *Interface::getAdapterList() {
	List *lst = 0;
	pcap_if_t *alldevs;
	pcap_if_t *d;
	//FIXME do something with this errbuf
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs(&alldevs, errbuf) != -1) {
		if(alldevs) {
			lst = new List(0, delete_info);
			for (d=alldevs; d; d=d->next)
				lst->add(new InterfaceInfo(d->name, d->description));
			pcap_freealldevs(alldevs);
		}
	}
	return lst;
}
/*
int Interface::updateFilters(DeviceContainer *cont) {
	char str[4096];
	//initializeFilter(str);
	bool flag = false;
	strcpy(str, "not ether src ");
	for(int i = 0;i < 4;i++) {
		DeviceInfo *dev = cont->getDeviceAtPos(i);
		if(dev) {
			if(flag)
				strcat(str, " and not ether src ");
			strcat(str, dev->getMACstr());
			flag = true;
		}
	}
	//printf(str);
	if(compileFilter(str) != -1) {
		if(setFilter() == -1) {
			return -2;
		}
	} else {
		return -1;
	}
	return 0;
}*/


void Interface::delete_info(void *obj) {
	delete (InterfaceInfo *)obj;
}

const char *Interface::getLastError() {
	return errbuf;
}

/*
 * Closes the interface
 * Parameters:
		none
 * Returns: void
*/
void Interface::close() {
	if(handle) {
		pcap_close(handle);
		handle = NULL;
	}
}
