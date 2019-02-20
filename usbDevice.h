#ifndef USBDEVICE__H
#define USBDEVICE__H

#include <libusb-1.0/libusb.h>
#include <byteVector.h>

#define USB_DEV_BUFFER_SIZE 1024
#define USB_DEB_BULK_BUFFER_SIZE 4096
#define USB_TIMEOUT 3000

class UsbDevice{

protected:

	static bool __usb_initialized;

	libusb_context *ctx;
	libusb_device_handle *handle;

	int UsbRead(ByteVector* data,int size,int timeout = USB_TIMEOUT);
	int UsbWrite(ByteVector* data);
	
	virtual bool GetDeviceManufacturer(libusb_device_handle *dev_handle, libusb_device_descriptor& desc,ByteVector* output);
	virtual bool GetDeviceDescription(libusb_device_handle *dev_handle, libusb_device_descriptor& desc,ByteVector* output);
	virtual bool GetDeviceEndpoints(libusb_device* device, libusb_device_descriptor& desc,ByteVector* output);

public:

	int USB_ENDPOINT_IN;
	int USB_ENDPOINT_OUT;
	
	bool debugCommunications;
	bool debugInitReleaseItems;

	UsbDevice();
	virtual ~UsbDevice();

	virtual int InitializeUsb();
	virtual int OpenDevice(unsigned int vid, unsigned int pid, int interface=0, int alternate=0);
	virtual bool ReleaseDevice();
	virtual bool CloseDevice();
	virtual bool FinalizeUsb();
	
	virtual bool DumpDevices();	

};

#endif
