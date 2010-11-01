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

#ifndef DEVICEBRIDGE_H_
#define DEVICEBRIDGE_H_

#ifdef _WIN32
#define WINVER 0x0501 //Windows XP
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <sys/times.h>
#endif
#include <time.h>
#include "Thread.h"
#include "Interface.h"
#include "DeviceInfo.h"
#include "EthPacket.h"
#include "Logger.h"
#include "UsbComm.h"
#include "Packet.h"


class DeviceBridge: public Thread {
public:
	typedef int (*CAPTURE_FUNC) (const u_char *, u_int);
	typedef int (*INJECT_FUNC) (EthPacket *);

private:
	static void capture_callback(u_char* user, const struct pcap_pkthdr* packet_header, const u_char* packet_data);
	CAPTURE_FUNC cap;
	INJECT_FUNC inj;
	bool capture_enabled;
	bool unregister;
	UsbComm *usb;
	Interface *eth;
	List *remote_mac;

private:
	static int compareFunc(void *, void *);
	static void deleteFunc(void *);
	void capture(const struct pcap_pkthdr* packet_header, const u_char* packet_data);
public:
	int run();
	DeviceBridge();
	bool startHandshake();
	bool makeBridge(Interface *eth, UsbComm *usb);
	bool makeBridge(const char *dev, UsbComm *usb);
	void removeBridge();
	void unregisterClient();
	void ignoreCapture() { capture_enabled = false; }
	void stop();
	void registerCaptureCallback(CAPTURE_FUNC func);
	void registerInjectCallback(INJECT_FUNC func);
	const char *getLastError();
	virtual ~DeviceBridge();
};

#endif /*DEVICEBRIDGE_H_*/
