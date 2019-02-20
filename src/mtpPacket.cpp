#include "mtpPacket.h"
#include "logger.h"

MtpPacket::MtpPacket(){
	containerLength = MTP_CONTAINER_MINIMAL_SIZE;
	containerType = MTP_CONTAINER_TYPE_COMMAND;
	mtpCode = MTP_GetDeviceInfo;
	transactionId = 0x00;
	
	payload = new ByteVector(16); // 4 + Uint32
}

MtpPacket::~MtpPacket(){
	// TODO: maybe add some actions on delete
	delete payload;
}

int MtpPacket::CalculateSize(){
	int result = MTP_CONTAINER_MINIMAL_SIZE + payload->size;
	return result;
}

bool MtpPacket::ToVector(ByteVector& destination){
	return ToVector(&destination);
}

bool MtpPacket::ToVector(ByteVector* destination){
	destination->Empty();
	destination->PushUint32((unsigned int) this->CalculateSize());
	destination->PushUint16(this->containerType);
	destination->PushUint16(this->mtpCode);
	destination->PushUint32(this->transactionId);
	destination->PushVectorData(this->payload);
	return true;
}

bool MtpPacket::FromVector(ByteVector& source){
	return FromVector(&source);
}

bool MtpPacket::FromVector(ByteVector* source){

	if(source->size<MTP_CONTAINER_MINIMAL_SIZE){
		// TODO: throw an exception, probably here
		return false;
	}
	
	this->containerLength = source->PopUint32();
	this->containerType = source->PopUint16();
	this->mtpCode = source->PopUint16();
	this->transactionId = source->PopUint32();
	
	this->payload->Empty();
	
	if(this->containerLength>MTP_CONTAINER_MINIMAL_SIZE){
		this->payload->PushVectorData(source);
	}
	
	// Some futher data settings
	this->hasDataResponse = (containerType == MTP_CONTAINER_TYPE_DATA);
	
	return true;
}

void MtpPacket::ClearPayload(){
	this->payload->Empty();
	

}

// Information packet

GetDeviceInformationPacket::GetDeviceInformationPacket(){
	this->requiresSession = false;
	this->mtpCode = MTP_GetDeviceInfo;
}

GetDeviceProperty::GetDeviceProperty(unsigned int propertyId){
	this->requiresSession = true;
	this->mtpCode = MTP_GetDevicePropValue;
	this->payload->Empty();
	this->payload->PushUint32(propertyId);

}
