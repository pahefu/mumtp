#ifndef PHONEDEVICE__H
#define PHONEDEVICE__H

#include <iostream>
#include <usbDevice.h>

#include <byteVector.h>

// Self storage
#include <storageInfo.h>

class PhoneDevice : public UsbDevice {

public:
	
	// Inner usb values
	unsigned int VID;
	unsigned int PID;
	unsigned int configurationId;
	unsigned int interfaceId;
	
	// Device objects
	StorageInfo storage;
	ByteVector deviceFriendlyName;
	ByteVector functionalMode;
	int batteryLevel;

	// Methods

	PhoneDevice();
	~PhoneDevice();


	bool SendPacket(ByteVector* data);
	bool ReceivePacket(ByteVector* data, int size = -1);
	bool FlushInput(); //TODO: remove this method
	

};

#endif
