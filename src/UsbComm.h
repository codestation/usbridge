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

#ifndef USBCOMM_H_
#define USBCOMM_H_

#include <libusb.h>

class UsbComm {
public:
	//typedef int(* usbcomm_cb)(int status, void *buffer, int size);

private:
	//struct internal_data {
	//	usbcomm_cb usb_cb;
	//	libusb_transfer *req_transfer;
	//};
	//static void transfer_callback(struct libusb_transfer *transfer);
	libusb_context *ctx;
	libusb_device_handle *devh;
	int ep_in;
	int ep_out;

public:
	UsbComm(): ctx(NULL), devh(NULL), ep_in(0), ep_out(0) {};
	bool init();
	bool open(int vendor, int product);
	int claim();
	int claim(int iface);
	int release();
	int release(int iface);
	int reset();
	void close();

	void set_endpoints(int in, int out);

	int bulk_read(unsigned char *data, int size, int *actual, int timeout);
	int bulk_write(unsigned char *data, int size, int *actual, int timeout);

	int bulk_read(int ep, unsigned char *data, int size, int *actual, int timeout);
	int bulk_write(int ep, unsigned char *data, int size, int *actual, int timeout);
	int bulk_transfer(libusb_endpoint_direction dir, int ep, unsigned char *data, int size, int *actual, int timeout);

	int set_configuration(int configuration);
	/*
	int bulk_read_async(usbcomm_cb cb, unsigned char *buffer, int size, int ep);
	int bulk_write_async(usbcomm_cb cb, unsigned char *buffer, int size, int ep);
	int bulk_transfer_async(usbcomm_cb cb, unsigned char *buffer, int size, libusb_endpoint_direction dir, int ep);
	void cancel_trasfer(void *id);
	bool handle_async_events();
	*/
	virtual ~UsbComm();
};

#endif /* USBCOMM_H_ */
