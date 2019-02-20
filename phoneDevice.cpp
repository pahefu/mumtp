#include "phoneDevice.h"


PhoneDevice::PhoneDevice(){
	VID = 0x00;
	PID = 0x00;

	// Parent usb values
	USB_ENDPOINT_IN = 0;
	USB_ENDPOINT_OUT = 0;
	
	batteryLevel = 0;

}

PhoneDevice::~PhoneDevice(){

}

bool PhoneDevice::SendPacket(ByteVector* data){

	int writeResult = this->UsbWrite(data);

	if(writeResult>0){
		return true;
	}
	
	return false;
}

bool PhoneDevice::ReceivePacket(ByteVector* data, int size){
	int readResult = this->UsbRead(data,size);
	return (readResult>0);
}


bool PhoneDevice::FlushInput(){
	
	//TODO: DEPRECATE THIS METHOD
	
	return true;
}
