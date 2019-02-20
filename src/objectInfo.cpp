#include <cstdio>
#include <cstring>

#include "objectInfo.h"
#include "logger.h"


ObjectInfo::ObjectInfo(unsigned int objectHandle){
	this->objectHandle = objectHandle;
	this->valid = false;
	
	this->filename = new ByteVector(32,16);
	this->dateCreated = new ByteVector(32,16);
	this->dateModified = new ByteVector(32,16);
	this->keywords = new ByteVector(32,16);
}

ObjectInfo::ObjectInfo(){
	this->objectHandle = 0x000000;
	this->valid = false;
	
	this->filename = new ByteVector(32,16);
	this->dateCreated = new ByteVector(32,16);
	this->dateModified = new ByteVector(32,16);
	this->keywords = new ByteVector(32,16);
}

ObjectInfo::~ObjectInfo(){
	delete this->filename;
	delete this->dateCreated;
	delete this->dateModified;
	delete this->keywords;
}

void ObjectInfo::FromVector(ByteVector& v){
	FromVector(&v);
}

void ObjectInfo::FromVector(ByteVector* v){
	valid = false;
	
	storageId = v->PopUint32();
	objectFormat = v->PopUint16();
	protectionStatus = v->PopUint16();
	objectCompressedSize = v->PopUint32();
	
	thumbFormat = v->PopUint16();
	thumbCompressedSize = v->PopUint32();
	thumbPixWidth = v->PopUint32();
	thumbPixHeight = v->PopUint32();
	
	imagePixWidth = v->PopUint32();
	imagePixHeight = v->PopUint32();
	imageBitDepth = v->PopUint32();
	
	parentObject = v->PopUint32();
	associationType = v->PopUint16();
	associationDescription = v->PopUint32();
	sequenceNumber = v->PopUint32();
	
	v->PopMTPUnicodeString(filename, false);
	v->PopMTPUnicodeString(dateCreated, true);
	v->PopMTPUnicodeString(dateModified, true);
	v->PopMTPUnicodeString(keywords, false);
	
	
	this->valid = true;
}

void ObjectInfo::DumpContent(){
	
	if(this->valid){			
		Logger::Log(Log_Normal,"Handle: %08X \t", this->objectHandle);
		Logger::Log(Log_Normal,"Object name: '");
		for(unsigned int i = 0;i<this->filename->size;i+=2){
			Logger::Log(Log_Normal,"%c", this->filename->memory[i]);
		}
		Logger::Log(Log_Normal,"' ");
		Logger::Log(Log_Normal,"\t Created: %s, ", this->dateCreated->memory);
		Logger::Log(Log_Normal,"\t Modified: %s\n", this->dateModified->memory);
	}
	
}

bool ObjectInfo::FileNameEquals(const char* str, int strLen){
	return this->filename->EqualsCharStr(str,strLen);
}

bool ObjectInfo::CloneFromOther(ObjectInfo* other){
	this->objectHandle = other->objectHandle;
	this->valid = other->valid;
	
	this->storageId = other->storageId;
	this->objectFormat = other->objectFormat;
	this->protectionStatus = other->protectionStatus;
	this->objectCompressedSize = other->objectCompressedSize;
	this->thumbFormat = other->thumbFormat;
	this->thumbCompressedSize = other->thumbCompressedSize;
	this->thumbPixWidth = other->thumbPixWidth;
	this->thumbPixHeight = other->thumbPixHeight;
	this->imagePixWidth = other->imagePixWidth;
	this->imagePixHeight = other->imagePixHeight;
	this->imageBitDepth = other->imageBitDepth;
	this->parentObject = other->parentObject;
	this->associationType = other->associationType;
	this->associationDescription = other->associationDescription;
	this->sequenceNumber = other->sequenceNumber;
	
	
	this->filename->Empty();
	this->filename->PushVectorData(other->filename);
	
	this->dateCreated->Empty();
	this->dateCreated->PushVectorData(other->dateCreated);
	
	this->dateModified->Empty();
	this->dateModified->PushVectorData(other->dateModified);
	
	this->keywords->Empty();
	this->keywords->PushVectorData(other->keywords);
	return true;
}

bool ObjectInfo::CloneFromOther(ObjectInfo& other){
	return CloneFromOther(&other);
}
