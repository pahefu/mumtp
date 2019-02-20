#include "storageInfo.h"
#include "logger.h"
#include <cstdio>
#include <wchar.h>
#include <locale.h>

StorageInfo::StorageInfo(){
	storageDescription = new ByteVector(50);
	volumeIdentifier = new ByteVector(50);
	
	storageId = 0x00;
	
	storageType = 0;
	filesystemType = 0;
	accessCapability = 0;
	maxCapacity = 0;
	freeSpaceInBytes = 0;
	freeSpaceInObjects = 0;
	
	humanMaxCapacity = 0;
	humanFreeSpace = 0;
	totalObjectsInStorage = 0;
	
}

StorageInfo::~ StorageInfo(){
	delete storageDescription;
	delete volumeIdentifier;
}

void StorageInfo::FromVector(ByteVector& v){
	FromVector(&v);
}

void StorageInfo::FromVector(ByteVector* v){
	this->valid = false;
	
	if(v->size>0){
		
		storageType = v->PopUint16();
		filesystemType = v->PopUint16();
		accessCapability = v->PopUint16();
		
		maxCapacity = v->PopUint64();
		freeSpaceInBytes = v->PopUint64();
		
		humanMaxCapacity = (maxCapacity>>20); // Round down to MB
		humanFreeSpace = (freeSpaceInBytes>>20); // Round down to MB
	
		freeSpaceInObjects = v->PopUint32();
		
		storageDescription->Empty();
		volumeIdentifier->Empty();

		unsigned char c = 0x00;
		// Grab the strings (read until utf8 null-t)
		
		int descriptionLength = v->PopByte();
		
		for(int i = 0;i<descriptionLength*2;i++){
			c = v->PopByte();
			storageDescription->PushByte(c);
		}
		
		int volumeLength = v->PopByte();
		
		for(int i = 0;i<volumeLength*2;i++){
			c = v->PopByte();
			volumeIdentifier->PushByte(c);
		}

		this->valid = true;
		
	}
	
}

void StorageInfo::DumpContent(){
	if(!this->valid){
		Logger::Log(Log_Warning,"Storage is not valid. Is the phone set to file transfer?\n");
	}else{
		Logger::Log(Log_Normal,"Storage ID: %08X\n" , storageId);
		Logger::Log(Log_Normal,"Storage Type: %s\n" , GetStorageTypeStr());
		Logger::Log(Log_Normal,"Filesystem Type: %s\n" , GetFilesystemTypeStr());
		
		#ifdef LC_ALL
			setlocale(LC_ALL,"");
		#endif

		ByteVector safeAsciiDescription(storageDescription->size);
		ByteVector safeAsciiVolume(volumeIdentifier->size);

		for(unsigned int i = 0;i<storageDescription->size;i++){
			if(storageDescription->memory[i]!=0x00){
				safeAsciiDescription.PushByte(storageDescription->memory[i]);
			}
		}
		
		for(unsigned int i = 0;i<volumeIdentifier->size;i++){
			if(volumeIdentifier->memory[i]!=0x00){
				safeAsciiVolume.PushByte(volumeIdentifier->memory[i]);
			}
		}

		Logger::Log(Log_Normal,"Storage description: %s\n" , safeAsciiDescription.memory);
		Logger::Log(Log_Normal,"Volume Identifier: %ls\n" , safeAsciiVolume.memory);
		
		
		
		Logger::Log(Log_Normal,"Capacity: %d MB\n" , humanMaxCapacity);
		Logger::Log(Log_Normal,"Free space: %d MB\n" , humanFreeSpace);
		Logger::Log(Log_Normal,"Free space in objects: %d \n" , freeSpaceInObjects);
		
	}
}

const char* StorageInfo::GetStorageTypeStr(){
	switch(this->storageType){
		case 0x0000: return "Undefined"; break;
		case 0x0001: return "Fixed ROM"; break;
		case 0x0002: return "Removable ROM"; break;
		case 0x0003: return "Fixed RAM"; break;
		case 0x0004: return "Removable RAM"; break;
		default: return "Reserved"; break;
	}
}

const char* StorageInfo::GetFilesystemTypeStr(){
	switch(this->filesystemType){
		case 0x0000: return "Undefined"; break;
		case 0x0001: return "Generic flat"; break;
		case 0x0002: return "Generic hierarchical"; break;
		case 0x0003: return "DCF"; break;
		default: return "Reserved or MTP-defined"; break;
	}
}

const char* StorageInfo::GetStorageDescriptionUtf8Str(){
	return (const char*) storageDescription->memory;
}

const char* StorageInfo::GetVolumeIdentifierUtf8Str(){
	return (const char*) volumeIdentifier->memory;
}
