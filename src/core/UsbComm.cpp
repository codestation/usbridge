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

UsbComm::~UsbComm() {
	libusb_exit(ctx);
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
