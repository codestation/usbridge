#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "Interface.h"
#include "ArgParser.h"
#include "List.h"
#include "UsbComm.h"
#include "DeviceBridge.h"
#include "Logger.h"

#define ID_VENDOR 0x054c
#define ID_PRODUCT 0x01c9

#define ENDPOINT_OUT 2
#define ENDPOINT_IN 129

ArgParser opts;
DeviceBridge *bridge = NULL;
bool end_loop = false;
char buffer[512];

void exit_signal(int signal) {
	printf("Trapped CTRL+C signal, shutting down...\n");
	if(bridge)
		bridge->removeBridge();
	end_loop = true;
}

int main(int argc, char **argv) {
	INFO_ON();
	if(!opts.parse(argc, argv)) {
		INFO("Starting bridge net-usb v0.01\n");
		INFO("Usage: bridge-usb [-i <interface>] [-l] [-v]\n\n");
		INFO("-i    Network device to use\n");
		INFO("-l    List network devices and exit\n");
		INFO("-v    Show verbose output (twice for debug output)\n");
		return 1;
	}
	if(opts.listInterfaces()) {
		List *lst = Interface::getAdapterList();
		if(!lst) {
			ERR("Cant get the interface list. Exiting...\n");
			return 1;
		}
		int i = 0;
		INFO("Interface list:\n");
		while(lst->hasNext()) {
			InterfaceInfo *inf = (InterfaceInfo *)lst->next();
			INFO("(%i) %s - %s\n", i++, inf->name, inf->desc);
		}
		delete lst;
		return 2;
	}
	if(!opts.verboseMode())
		INFO_OFF();
	if(opts.verboseMode() > 1)
		DEBUG_ON();
	INFO("bridge-usb v0.01 starting...\n");

	if(!opts.interfaceName()) {
		ERR("No interface defined, exiting...\n");
		return 1;
	}
	UsbComm *usb = new UsbComm();
	usb->init();
	usb->set_endpoints(ENDPOINT_IN, ENDPOINT_OUT);
	INFO("Installing signal handler...\n");
	signal(SIGINT, exit_signal);
	INFO("Trying to open USB device...\n");
	while(!end_loop) {
		while(!end_loop && !usb->open(ID_VENDOR, ID_PRODUCT)) {
			sleep(1);
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
		if(!usb->claim()) {
			bridge = new DeviceBridge();
			INFO("Making bridge usb <==> eth\n");
			if(!bridge->makeBridge(opts.interfaceName(), usb))
				ERR("Error while making bridge\n");
			INFO("Bridge closed. Freeing resources\n");
			delete bridge;
			bridge = NULL;
			usb->release();
		} else {
			ERR("Claim USB device failed\n");
			end_loop = true;
		}
		INFO("Reset USB interface\n");
		usb->reset();
		INFO("Closing USB device\n");
		usb->close();
	}
	delete usb;
	return 0;
}
