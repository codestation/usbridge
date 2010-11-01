/*
 * UsbComm.cpp
 *
 *  Created on: 30/10/2010
 *      Author: code
 */

#include "UsbComm.h"

bool UsbComm::init() {
	if(libusb_init(&ctx) >= 0) {
		libusb_set_debug(ctx, 3);
		return true;
	}
	return false;
}

bool UsbComm::open(int vendor, int product) {
	devh = libusb_open_device_with_vid_pid(ctx, vendor, product);
	return (devh);
}

int UsbComm::bulk_read(int ep, unsigned char *data, int size, int *actual, int timeout) {
	return bulk_transfer(LIBUSB_ENDPOINT_IN, ep, data, size, actual, timeout);
}

int UsbComm::bulk_write(int ep, unsigned char *data, int size, int *actual, int timeout) {
	return bulk_transfer(LIBUSB_ENDPOINT_OUT, ep, data, size, actual, timeout);
}

int UsbComm::bulk_read(unsigned char *data, int size, int *actual, int timeout) {
	return bulk_read(ep_in, data, size, actual, timeout);
}

int UsbComm::bulk_write(unsigned char *data, int size, int *actual, int timeout) {
	return bulk_write(ep_out, data, size, actual, timeout);
}

int UsbComm::bulk_transfer(libusb_endpoint_direction dir, int ep, unsigned char *data, int size, int *actual, int timeout) {
	return libusb_bulk_transfer(devh, dir | ep, data, size, actual, timeout);
}

int UsbComm::reset() {
	return libusb_reset_device(devh);
}

void UsbComm::close() {
	libusb_close(devh);
}

int UsbComm::claim() {
	return claim(0);
}

int UsbComm::claim(int iface) {
	return libusb_claim_interface(devh, iface);
}

int UsbComm::release() {
	return release(0);
}

int UsbComm::release(int iface) {
	return libusb_release_interface(devh, iface);
}

int UsbComm::set_configuration(int config) {
	return libusb_control_transfer(devh, LIBUSB_RECIPIENT_DEVICE | LIBUSB_REQUEST_TYPE_STANDARD | LIBUSB_ENDPOINT_OUT, LIBUSB_REQUEST_SET_CONFIGURATION, config, 0, 0, 0, 0);
}

void UsbComm::set_endpoints(int in, int out) {
	ep_in = in;
	ep_out = out;
}

/*
int UsbComm::bulk_transfer_async(usbcomm_cb cb, unsigned char *buffer, int size, libusb_endpoint_direction dir, int ep) {
	libusb_transfer* transfer = libusb_alloc_transfer(0);
	if(transfer) {
		struct internal_data *data = new internal_data;
		data->req_transfer = transfer;
		data->usb_cb = cb;
		libusb_fill_bulk_transfer(transfer, devh, ep_addr | mode, buffer, size, transfer_callback, data, 0);
		libusb_submit_transfer(transfer);
		return data;
	}
	return NULL;
}

void UsbComm::transfer_callback(libusb_transfer *transfer) {
	internal_data *data = (internal_data *)transfer->user_data;
	if(data->usb_cb)
		data->usb_cb(transfer->status, transfer->buffer, transfer->actual_length);
	if(transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		printf("usb_transfer_failed\n");
		libusb_free_transfer(transfer);
		delete (UsbComm::internal_data *)transfer->user_data;
	}
	printf("usb_transfer_complete\n");
	if(!(transfer->endpoint | LIBUSB_ENDPOINT_OUT))
	libusb_submit_transfer(data->req_transfer);
}

bool UsbComm::handle_async_events() {
	return libusb_handle_events(ctx) >= 0;
}

void UsbComm::cancel_trasfer(void *id) {
	libusb_cancel_transfer((libusb_transfer *)id);
}*/

UsbComm::~UsbComm() {
	libusb_exit(ctx);
}
