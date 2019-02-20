#ifndef MTPPACKET__H
#define MTPPACKET__H

#include <byteVector.h> // Included vector here
#include <mtpCommands.h>

#define MTP_CONTAINER_MINIMAL_SIZE 12

#define MTP_CONTAINER_TYPE_COMMAND 1
#define MTP_CONTAINER_TYPE_DATA 2
#define MTP_CONTAINER_TYPE_RESPONSE 3
#define MTP_CONTAINER_TYPE_EVENT 4

class MtpPacket{

public:

	bool hasDataResponse;
	bool requiresSession;

	unsigned int containerLength;
	unsigned short containerType;
	unsigned short mtpCode;
	unsigned int transactionId;
	ByteVector* payload;

	MtpPacket();
	virtual ~MtpPacket();

	void ClearPayload();

	int CalculateSize();
	bool ToVector(ByteVector& destination);
	bool ToVector(ByteVector* destination);
	bool FromVector(ByteVector& source);
	bool FromVector(ByteVector* source);

};

// All the packets required


class GetDeviceInformationPacket : public MtpPacket{
public:
	GetDeviceInformationPacket();
};

class OpenSessionPacket : public MtpPacket{
public:
	OpenSessionPacket();
};

class CloseSessionPacket : public MtpPacket{
public:
	CloseSessionPacket();
};

class GetStoragesPacket : public MtpPacket{
public:
	GetStoragesPacket();
};

class GetDeviceProperty : public MtpPacket{
public:
	GetDeviceProperty(unsigned int propertyId);
};

#endif
