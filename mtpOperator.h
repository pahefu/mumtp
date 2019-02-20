#ifndef MTPOPERATOR__H
#define MTPOPERATOR__H

#include <mtpConstants.h>
#include <mtpPacket.h>
#include <mtpDeviceProperties.h>
#include <objectInfo.h>
#include <fileWriter.h>
#include <phoneDevice.h>

#include <vector>


enum OperationErrors{
	ERR_None = 0,
	ERR_PacketNotReceived = 1,
	ERR_ResponseFoundWithDataRequest = 2,
	ERR_PacketNotReceivedAfterData = 3
};


class MtpOperator{

private:
	void ResetIds();

	bool SendPacket(MtpPacket& packet, bool transactionChange=true);
	bool ReadPacket(int size=-1);
	
	int ReadDataAndResponse();
	int ReadResponse();
	
	int ReadBulkDataAndResponse(void* destinationPtr, bool toMemory);


public:

	MtpPacket writingPackage;
	MtpPacket readingPackage;
	
	// Communications buffers
	ByteVector* writingBuffer;
	ByteVector* readingBuffer;
	ByteVector* temporaryPayload;
	
	// Data buffer to reuse
	ByteVector* reusableBuffer;
	
	PhoneDevice device;

	unsigned int expectedTransactionId;
	unsigned int transactionId;
	unsigned int sessionID;
	bool sessionOpen;
	
	ObjectInfo currentObject; // To point to
	
	int errorCode;

	MtpOperator();
	~MtpOperator();

	// Normal operations
	
	int GetDeviceInfo();
	int CheckSessionOpen();
	
	int OpenSession();
	int CloseSession();
	
	int SyncStorages();
	int SyncStorageById(unsigned int storageId);
	int SyncStorageObjectCount();
	int GetFolderHandles(std::vector<unsigned int>& handles,unsigned int storageId, unsigned int folderId, unsigned int typeId=MTP_ALL_FILE_TYPES);
	ObjectInfo* GetObjectInfo(unsigned int objectHandle);

	int GetDeviceProperty(unsigned int propertyId);
	int SyncBatery();
	int SyncDeviceFriendlyName();
	int SyncFunctionalMode();
	
	// More humanlike instructions
	int GetHandleByName(ObjectInfo* inOutputObject, const char* name, int nameLength);
	
	int CdToPath(const char* pathStr,int pathLength); // Easy movement within handles
	int GetPathObjects(std::vector<ObjectInfo*>& infos, unsigned int handlePath);
	
	int DownloadFile(const char* pathStr, int pathLength);
	int DownloadFile(unsigned int handle, FileWriter* writerPtr=NULL);
	
};

#endif
