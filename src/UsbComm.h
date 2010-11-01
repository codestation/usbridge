/*
 * UsbComm.h
 *
 *  Created on: 30/10/2010
 *      Author: code
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
