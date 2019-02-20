#include <cstdio>


#include "returnConstants.h"
#include "mtpOperator.h"
#include "mtpConstants.h"
#include "mtpObjectFormats.h"
#include "fileWriter.h"

#include "logger.h"
#include "pathHandler.h"

MtpOperator::MtpOperator(){

	writingBuffer = new ByteVector(100);
	readingBuffer = new ByteVector(512,4);
	temporaryPayload = new ByteVector(500* 1024);
	
	reusableBuffer = new ByteVector(100*1024,16);
	ResetIds();
}

MtpOperator::~MtpOperator(){
	delete writingBuffer;
	delete readingBuffer;
	delete temporaryPayload;
	
	delete reusableBuffer;
}


void MtpOperator::ResetIds(){
	expectedTransactionId = 0x00;
	transactionId = 0x00;
	sessionID = 0x00;
	sessionOpen = false;

}

bool MtpOperator::SendPacket(MtpPacket& packet, bool transactionChange){
	if(transactionChange){
		transactionId++;
	}
	expectedTransactionId = transactionId; // Need this to be expected
	packet.transactionId = transactionId;
	
	packet.ToVector(writingBuffer);
	bool result = device.SendPacket(writingBuffer);
	
	packet.ClearPayload(); // Clear after sending, makes this not needed on every commmand
	return result;
}

bool MtpOperator::ReadPacket(int size){
	bool dataRead = false;
	
	while(!dataRead){
		dataRead = device.ReceivePacket(readingBuffer,size);
		if(!dataRead){ // Cannot read from device, go away!!
			break;
		}
		
		readingPackage.FromVector(readingBuffer);
		
		if(readingPackage.transactionId!=expectedTransactionId){
			Logger::Log(Log_Debug,"[MTP Operator] Unexpected transaction received: %08X, expecting %08X\n", readingPackage.transactionId, expectedTransactionId);
			dataRead = false; // Get this packet out, not for us, previous transaction!!
		}
	}
	return dataRead;
}

int MtpOperator::ReadDataAndResponse(){
	errorCode = ERR_None;
	if(!ReadPacket()){
		errorCode = ERR_PacketNotReceived;
		return false;
	}
	
	if(!readingPackage.hasDataResponse){
		errorCode = ERR_ResponseFoundWithDataRequest;
		return false;
	}
	
	temporaryPayload->Empty();
	temporaryPayload->PushVectorData(readingPackage.payload);
	
	if(!ReadPacket()){
		errorCode = ERR_PacketNotReceivedAfterData;
		return false;
	}

	return true;
	
}

int MtpOperator::ReadResponse(){
	errorCode = ERR_None;
	if(!ReadPacket()){
		errorCode = ERR_PacketNotReceived;
		return false;
	}
	return true;
	
}

int MtpOperator::ReadBulkDataAndResponse(void* destinationPtr, bool toMemory){
	errorCode = ERR_None;
	if(!ReadPacket(512)){ // Not receiving the first data package. BAD THING!
		errorCode = ERR_PacketNotReceived;
		return false;
	}
	
	if(!readingPackage.hasDataResponse){ // Read a package, and there was no data, but RESPONSE!!
		errorCode = ERR_ResponseFoundWithDataRequest;
		Logger::Log(Log_Normal,"[MTP Operator] Failed to read bulk data. Response found\n");
		return false;
	}

	unsigned int payloadSize = readingPackage.containerLength - MTP_CONTAINER_MINIMAL_SIZE;
	unsigned int payloadRead = 0x00;
	
	bool readingLoopOn = true;
	
	//Logger::Log(Log_Normal,"[MTP Operator] Remaining bytes in first DATA message read: %d \n", readingPackage.payload->size);
	
	// Copying the initial payload (up to remaining to the destination 
	if(toMemory){
		ByteVector* destination = (ByteVector*) destinationPtr;
		payloadRead+=readingPackage.payload->size;
		destination->PushVectorData(readingPackage.payload);
		if(destination->size == payloadSize){ // If all the payload fit in the remaining data, dont read anymore
			readingLoopOn = false;
		}
	}else{
		FileWriter* fileWritter = (FileWriter*) destinationPtr;
		payloadRead+=readingPackage.payload->size;
		fileWritter->WriteToFile(readingPackage.payload->memory, readingPackage.payload->size);
		if(fileWritter->totalWritten == payloadSize){ // If all the payload fit in the remaining data, dont read anymore
			readingLoopOn = false;
		}
	}

	// Obtaining the remaining bytes from the device (if there are any)

	while(readingLoopOn){
		//Logger::Log(Log_Normal,"[MTP Operator] Reading bulk data\n");
		
		temporaryPayload->Empty();
		unsigned int whatToRead = (payloadSize-payloadRead);
		if(whatToRead>temporaryPayload->capacity){
			whatToRead = temporaryPayload->capacity;
		}
		
		
		if(!device.ReceivePacket(temporaryPayload, whatToRead)){
			break;
		}

		if(toMemory){
			ByteVector* destination = (ByteVector*) destinationPtr;
			payloadRead+=temporaryPayload->size;
			destination->PushVectorData(temporaryPayload);
			if(destination->size == payloadSize){
				readingLoopOn = false;
			}

		}else{
			FileWriter* fileWritter = (FileWriter*) destinationPtr;		
			payloadRead+=temporaryPayload->size;
			fileWritter->WriteToFile(temporaryPayload->memory, temporaryPayload->size);
			
			double percent = (fileWritter->totalWritten*100.0 / payloadSize);
			
			Logger::Log(Log_Debug,"Downloading %.02f%\n", percent);
			if(fileWritter->totalWritten>= payloadSize){
				readingLoopOn = false;
			}
		}
		
		//Logger::Log(Log_Normal,"[MTP Operator] Reading bulk data END: %d\n", readingLoopOn);
	}

	if(!ReadResponse()){
		return false;
	}
	
	// CHECK the outcome
	//Logger::Log(Log_Normal,"[MTP Operator] Bulk read: %08X, payload request size %08X\n", payloadRead, payloadSize);
	if(payloadRead == payloadSize){
		return true;
	}
	
	return false;
}


////////////////

int MtpOperator::CheckSessionOpen(){
	if(sessionOpen){
		return MUMTP_SUCCESS;
	}

	if(OpenSession()!=MUMTP_SUCCESS){
		return MUMTP_OPEN_SESSION_FAILED;
	}
	
	return MUMTP_SUCCESS;
}


int MtpOperator::GetDeviceInfo(){
	writingPackage.mtpCode = MTP_GetDeviceInfo;
	if(!SendPacket(writingPackage)){
		return MUMTP_SEND_PACKET_FAILED;
	}
	
	if(!ReadDataAndResponse()){
		return MUMTP_RECEIVE_DATA_FAILED;
	}
	
	// Parse stuff in the payload, here,  if required
	return MUMTP_SUCCESS;
}

int MtpOperator::OpenSession(){
	sessionID++;
	writingPackage.mtpCode = MTP_OpenSession;
	writingPackage.payload->PushUint32(sessionID);
	if(!SendPacket(writingPackage)){
		return MUMTP_SEND_PACKET_FAILED;
	}
		
	if(!ReadResponse()){
		return MUMTP_RECEIVE_DATA_FAILED;
	}

	if(readingPackage.mtpCode == MTP_ANS_OK || readingPackage.mtpCode == MTP_ANS_SESSION_ALREADY_OPEN){
		sessionOpen = true;
		return MUMTP_SUCCESS;
	}
	return MUMTP_OPERATION_FAILED;
}

int MtpOperator::CloseSession(){
	writingPackage.mtpCode = MTP_CloseSession;
	writingPackage.payload->PushUint32(sessionID);
	if(!SendPacket(writingPackage)){
		return MUMTP_SEND_PACKET_FAILED;
	}
		
	if(!ReadResponse()){
		return MUMTP_RECEIVE_DATA_FAILED;
	}
	
	if(readingPackage.mtpCode == MTP_ANS_OK || readingPackage.mtpCode == MTP_ANS_SESSION_NOT_OPEN){
		sessionOpen = false;
		return MUMTP_SUCCESS;
	}
	return MUMTP_OPERATION_FAILED;
}

int MtpOperator::SyncStorages(){
	if(CheckSessionOpen()!=MUMTP_SUCCESS){
		return MUMTP_OPEN_SESSION_FAILED;
	}

	writingPackage.mtpCode = MTP_GetStorageIDs;
	if(!SendPacket(writingPackage)){
		return MUMTP_STORAGE_SYNC_FAILED;
	}

	device.storage.valid = false; // Set the storage to false, until it is queried
	
	if(!ReadDataAndResponse()){	
		if(errorCode == ERR_ResponseFoundWithDataRequest){
			return MUMTP_STORAGE_SYNC_FAILED;
		}
	}

	bool storageIdAvailable = false;
	
	if(temporaryPayload->size>0){
		unsigned int totalStorages = temporaryPayload->PopUint32();
		if(totalStorages>0){
			device.storage.storageId = temporaryPayload->PopUint32();
			storageIdAvailable = true;
		}
	}
		
	// If there is storage data available, go get it
	if(storageIdAvailable){
		return SyncStorageById(device.storage.storageId);
		
	}

	return MUMTP_STORAGE_SYNC_FAILED;
}

int MtpOperator::SyncStorageById(unsigned int storageId){

	writingPackage.mtpCode = MTP_GetStorageInfo;
	writingPackage.payload->PushUint32(device.storage.storageId);
	if(!SendPacket(writingPackage)){
		return MUMTP_SEND_PACKET_FAILED;	
	}
	
	if(!ReadDataAndResponse()){
		return MUMTP_RECEIVE_DATA_FAILED;
	}
	
	device.storage.FromVector(temporaryPayload);
	return MUMTP_SUCCESS; // All was ok!

}

int MtpOperator::SyncStorageObjectCount(){
	
	if(!device.storage.valid){
		return MUMTP_INVALID_STORAGE;
	}

	device.storage.totalObjectsInStorage = 0;
	
	writingPackage.mtpCode = MTP_GetNumObjects;
	writingPackage.payload->PushUint32(0xFFFFFFFF); // << All storages together
	writingPackage.payload->PushUint32(0x00000000); // Ignore types, get all
	writingPackage.payload->PushUint32(0x00000000); // No specific folder set, get all
	if(!SendPacket(writingPackage)){
		return MUMTP_SEND_PACKET_FAILED;	
	}
	
	if(!ReadPacket()){ // storage data
		return MUMTP_RECEIVE_DATA_FAILED;
	}

	if(readingPackage.payload->size>0){
		device.storage.totalObjectsInStorage = readingPackage.payload->PopUint32();
	}
	return MUMTP_SUCCESS;
	
}

int MtpOperator::GetDeviceProperty(unsigned int propertyId){
	if(CheckSessionOpen()!=MUMTP_SUCCESS){
		return MUMTP_SESSION_IS_NOT_OPEN;
	}
	
	writingPackage.mtpCode = MTP_GetDevicePropValue;
	writingPackage.payload->PushUint32(propertyId);
	if(!SendPacket(writingPackage)){
		return MUMTP_SEND_PACKET_FAILED;
	}
	
	if(!ReadDataAndResponse()){	
		if(errorCode == ERR_ResponseFoundWithDataRequest){
			if(readingPackage.mtpCode == MTP_ANS_DEVICEPROP_NOT_SUPPORTED){
				return MUMTP_UNSUPPORTED_PROPERTY_ID;
			}
		}
		return MUMTP_RECEIVE_DATA_FAILED;
	}

	return MUMTP_SUCCESS;
}


int MtpOperator::SyncBatery(){
	
	int result = GetDeviceProperty(MTP_DEV_PROP_BatteryLevel);
	if(result == MUMTP_SUCCESS){
		if(temporaryPayload->size>=1){
			device.batteryLevel = temporaryPayload->PopByte();
		}
	}
	return result;
}

int MtpOperator::SyncDeviceFriendlyName(){
	device.deviceFriendlyName.Empty();
	int result = GetDeviceProperty(MTP_DEV_PROP_DeviceFriendlyName);
	if(result == MUMTP_SUCCESS){
		if(temporaryPayload->size>=1){
			unsigned int nameLength = temporaryPayload->PopByte();
			for(unsigned int i = 0;i<nameLength;i++){
				// TODO: here ASCII values are taken, which should be Unicode2byte and so on
				device.deviceFriendlyName.PushByte(temporaryPayload->PopByte());
				temporaryPayload->PopByte(); // << Ignored 
			}
		}
	}
	return result;
}

int MtpOperator::SyncFunctionalMode(){
	int result = GetDeviceProperty(0xD403);
	if(result){
		if(temporaryPayload->size>=1){
			temporaryPayload->DumpContent();
		}
	}
	return result;
}

int MtpOperator::GetFolderHandles(std::vector<unsigned int>& handles, unsigned int storageId, unsigned int folderId, unsigned int typeId){
	
	handles.clear();
	
	if(CheckSessionOpen()!=MUMTP_SUCCESS){
		return MUMTP_SESSION_IS_NOT_OPEN;
	}
	
	writingPackage.mtpCode = MTP_GetObjectHandles;
	writingPackage.payload->PushUint32(storageId);
	writingPackage.payload->PushUint32(typeId); // All type of handles by default
	writingPackage.payload->PushUint32(folderId);
	if(!SendPacket(writingPackage)){
		return MUMTP_SEND_PACKET_FAILED;
	}
	

	if(!ReadBulkDataAndResponse(reusableBuffer, true)){
		//Logger::Log(Log_Debug, "[MTP Operator] Couldnt fetch folder handles for folder: %08X\n", folderId);		
		return MUMTP_RECEIVE_DATA_FAILED;
	}
	
	if(reusableBuffer->size<4){ // Not even the minimum count of handles
		return MUMTP_INVALID_DATA_READ;
	}
	
	unsigned int totalHandles = reusableBuffer->PopUint32();
	//Logger::Log(Log_Debug, "[MTP Operator] Total handles: %08X\n", totalHandles);		
	for(unsigned int i = 0; i < totalHandles;i++){
		handles.push_back(reusableBuffer->PopUint32());
	}
	
	return MUMTP_SUCCESS;
}

ObjectInfo* MtpOperator::GetObjectInfo(unsigned int objectHandle){

	if(CheckSessionOpen()!=MUMTP_SUCCESS){
		return NULL;
	}
	
	writingPackage.mtpCode = MTP_GetObjectInfo;
	writingPackage.payload->PushUint32(objectHandle);
	if(!SendPacket(writingPackage)){
		return NULL;
	}
	
	if(!ReadBulkDataAndResponse(reusableBuffer, true)){
		Logger::Log(Log_Debug, "[MTP Operator] Couldnt GetObjectInfo for handle: %08X\n", objectHandle);		
		return NULL;
	}
	
	ObjectInfo* info = new ObjectInfo(objectHandle);
	info->FromVector(reusableBuffer);
	return info;
}

int MtpOperator::CdToPath(const char* pathStr, int pathLength){
	
	if(!device.storage.valid){
		return MUMTP_INVALID_STORAGE;
	}
	
	std::vector<std::string> folders = PathHandler::SplitPath(pathStr,pathLength);
	std::vector<unsigned int> handles;
	
	ObjectInfo localPath;
	localPath.CloneFromOther(this->currentObject);
		
	for(unsigned int i = 0;i<folders.size();i++){
		std::string temp = folders.at(i);
		
		if(temp=="/" && i==0){ // skip the root folder, as we could always go there
			localPath.objectHandle = MTP_ROOT_FOLDER;
			continue;
		}
		
		if(temp==""){ // skip empty paths (probably created when splitting)
			continue;
		}
			
		if(GetHandleByName(&localPath,temp.c_str(), temp.length())!=MUMTP_SUCCESS){ 
			return MUMTP_HANDLE_NOT_FOUND;
		}
	}
	
	// Revert back to the good one
	currentObject.CloneFromOther(localPath);
	return MUMTP_SUCCESS;
}

int MtpOperator::GetHandleByName(ObjectInfo* inOutputObject, const char* name, int nameLength){
	if(!device.storage.valid){
		return MUMTP_INVALID_STORAGE;
	}
	
	if(inOutputObject==NULL){
		return MUMTP_INVALID_INPUTOUTPUT_OBJECT;
	}
	
	std::vector<ObjectInfo*> objectsInfo;
	
	if(GetPathObjects(objectsInfo, inOutputObject->objectHandle)!=MUMTP_SUCCESS){
		return MUMTP_HANDLE_CHILDREN_NOT_FOUND;
	}

	bool found = false;
	for(unsigned int j = 0;j<objectsInfo.size();j++){
		ObjectInfo* infoPtr = objectsInfo.at(j);	
		if(!found && infoPtr->filename->EqualsCharStr(name, nameLength)){
			inOutputObject->CloneFromOther(infoPtr);
			found = true;
		}
		delete infoPtr;
	}

	return ((found) ? MUMTP_SUCCESS : MUMTP_HANDLE_NOT_FOUND);
}

int MtpOperator::DownloadFile(const char* pathStr, int pathLength){
	
	ObjectInfo backupObject;
	backupObject.CloneFromOther(this->currentObject);
	
	int result = MUMTP_SUCCESS;
	if(CheckSessionOpen()!=MUMTP_SUCCESS){
		return MUMTP_SESSION_IS_NOT_OPEN;
	}
	
	if(!device.storage.valid){
		return MUMTP_INVALID_STORAGE;
	}
	
	if(this->CdToPath(pathStr, pathLength)){ // Virtually, we are instead cd-ing to a file handle :)
		std::string fileName = PathHandler::GetLastPart(pathStr, pathLength);
		
		if(this->currentObject.objectFormat == MTP_OBJECT_FORMAT_FOLDER){
			return MUMTP_REQUIRED_DOWNLOAD_IS_FOLDER;
		}
		
		FileWriter writer;
		if(writer.OpenFile(fileName.c_str(), fileName.length(),"wb+")){
			result = DownloadFile(this->currentObject.objectHandle, &writer);
		}
	}else{
		result = MUMTP_HANDLE_NOT_FOUND;
	}
	
	// Go back to previous state
	this->currentObject.CloneFromOther(backupObject);
	return result;
}

int MtpOperator::DownloadFile(unsigned int handle, FileWriter* writerPtr){
	
	if(CheckSessionOpen()!=MUMTP_SUCCESS){
		return MUMTP_SESSION_IS_NOT_OPEN;
	}
	
	if(!device.storage.valid){
		return MUMTP_INVALID_STORAGE;
	}
	int result = MUMTP_SUCCESS;
	bool selfHandling = (writerPtr==NULL);
	
	ObjectInfo* info = (handle!=this->currentObject.objectHandle) ? GetObjectInfo(handle) : (&this->currentObject);
	unsigned int objectFormat = MTP_OBJECT_FORMAT_FOLDER;
	std::string fileName = "";
	if(info!=NULL){
		for(unsigned int i = 0;i<info->filename->size;i+=2){
			fileName+=info->filename->memory[i];
		}
		objectFormat = info->objectFormat;
		if(info!= (&this->currentObject)){
			delete info;
		}
	}
	
	if(fileName=="" && writerPtr==NULL){ // A writer has not been passed and filename wasnt set. time to go!
		return MUMTP_NO_WRITER_NOR_FILENAME_RECV;
	}

	
	if(objectFormat == MTP_OBJECT_FORMAT_FOLDER){ // Trying to download a folder, that's not gonna work!
		return MUMTP_REQUIRED_DOWNLOAD_IS_FOLDER;
	}
	
	if(selfHandling){ // Self handle the fileName
		writerPtr = new FileWriter();
		writerPtr->OpenFile(fileName.c_str(), fileName.length(),"wb+");
	}

	if(writerPtr->fileOpen){
		writingPackage.mtpCode = MTP_GetObject;
		writingPackage.payload->PushUint32(handle);
		if(SendPacket(writingPackage)){				
			Logger::Log(Log_Debug, "[MTP Operator] Reading file contents\n");
			if(ReadBulkDataAndResponse(writerPtr, false)){
				result = MUMTP_SUCCESS;
			}
		}
	}
	
	if(selfHandling){ // clean our mess, if any
		delete writerPtr;
	}
	
	return result;
	
}

int MtpOperator::GetPathObjects(std::vector<ObjectInfo*>& infos, unsigned int handlePath){
	if(!device.storage.valid){
		return MUMTP_INVALID_STORAGE;
	}
	
	infos.clear();
	
	std::vector<unsigned int> handles;
	int handlesResult = GetFolderHandles(handles, device.storage.storageId, handlePath);
	if(handlesResult!=MUMTP_SUCCESS){
		return MUMTP_HANDLE_CHILDREN_NOT_FOUND;
	}
	
	for(unsigned int j = 0;j<handles.size();++j){
		ObjectInfo* infoPtr = GetObjectInfo(handles.at(j));
		if(infoPtr!=NULL){
			infos.push_back(infoPtr);
		}else{
			//Logger::Log(Log_Debug, "[MTP Operator] [*Warning*] Grabbed a NULL object when quering handle 0x%08\n", handles.at(j));
		}		
	}
	return MUMTP_SUCCESS;
}
