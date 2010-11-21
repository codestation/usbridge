/*
 *  usbridge project (temporary name)
 *
 *  Copyright (C) 2010  Codestation
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

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "core/Interface.h"
#include "core/GetOpt.h"
#include "core/List.h"
#include "core/Logger.h"
#include "core/UsbCommAdapter.h"
#include "core/InterfaceAdapter.h"

#define ID_VENDOR 0x054c
#define ID_PRODUCT 0x01c9

#define ENDPOINT_OUT 2
#define ENDPOINT_IN 129

bool end_loop = false;
char buffer[512];
UsbCommAdapter *usb = NULL;
InterfaceAdapter *inter = NULL;

void exit_signal(int signal) {
	printf("Trapped CTRL+C signal, shutting down...\n");
	if(usb)
		usb->close();
	if(inter)
		inter->close();
	end_loop = true;
}

void usage() {
	INFO("bridge net-usb v0.01\n");
	INFO("Usage: bridge-usb [-i <interface>] [-l] [-v]\n\n");
	INFO("-i    Network device to use\n");
	INFO("-l    List network devices and exit\n");
	INFO("-v    Show verbose output (twice for debug output)\n");
}

int main(int argc, char **argv) {
	char option = 0;
	int verbose = 0;
	bool empty = true;
	bool list_interfaces = false;
	const char *inter_name = NULL;

	GetOpt opt(argc, argv, "i:vl");
	while(option != '?' && (option = opt()) != EOF) {
		switch(option) {
		case 'v': verbose++; break;
		case 'l': list_interfaces = true; break;
		case 'i': inter_name = opt.arg(); break;
		case '?':
			INFO("\n");
			usage();
		default:
			return EXIT_FAILURE;
		}
		empty = false;
	}
	INFO_ON();

	if(empty) {
		usage();
		return EXIT_FAILURE;
	}
	if(list_interfaces) {
		List *lst = Interface::getAdapterList();
		if(!lst) {
			ERR("Cant get the interface list. Exiting...\n");
			return EXIT_FAILURE;
		}
		int i = 0;
		INFO("Interface list:\n");
		while(lst->hasNext()) {
			InterfaceInfo *inf = (InterfaceInfo *)lst->next();
			INFO("(%i) %s - %s\n", i++, inf->name, inf->desc);
		}
		delete lst;
		return EXIT_SUCCESS + 2;
	}

	if(!verbose)
		INFO_OFF();
	if(verbose > 1)
		DEBUG_ON();

	INFO("bridge net-usb v0.01 starting...\n");

	usb->set_endpoints(ENDPOINT_IN, ENDPOINT_OUT);
	INFO("Installing signal handler...\n");
	signal(SIGINT, exit_signal);
	INFO("Trying to open USB device...\n");
	while(!end_loop) {
		while(!end_loop && !usb->UsbComm::open(ID_VENDOR, ID_PRODUCT)) {
#ifdef _WIN32
			Sleep(1000);
#else
			sleep(1);
#endif
		}
		if(end_loop)
			break;
		INFO("PSP Detected!\n");
		INFO("Checking USB device...");
		if(usb->set_configuration(1) < 0) {
			INFO("Failed, aborting\n");
			break;
		}
		INFO("Success\n");
		if(!usb->UsbComm::claim()) {
			INFO("Making bridge usb <==> eth\n");
			if(!(inter->out(usb) && usb->out(inter)))
				ERR("Error while making bridge\n");
			INFO("Bridge closed\n");
			usb->UsbComm::release();
		} else {
			ERR("Claim USB device failed\n");
			end_loop = true;
		}
		usb->UsbComm::reset();
		usb->UsbComm::close();
	}
	delete inter;
	delete usb;
	return 0;
}
