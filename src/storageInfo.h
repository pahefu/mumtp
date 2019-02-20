#ifndef STORAGEINFO__H
#define STORAGEINFO__H

#include <byteVector.h>

class StorageInfo{

public:
		bool valid;
		unsigned int storageId;
		unsigned short storageType;
		unsigned short filesystemType;
		unsigned short accessCapability;
		uint64_t maxCapacity;
		uint64_t freeSpaceInBytes;
		uint64_t freeSpaceInObjects;
		
		ByteVector* storageDescription;
		ByteVector* volumeIdentifier;
				
		unsigned int humanMaxCapacity;
		unsigned int humanFreeSpace;
		
		// Extra info
		
		unsigned int totalObjectsInStorage;

		StorageInfo();
		~StorageInfo();

		void FromVector(ByteVector& v);
		void FromVector(ByteVector* v);
		void DumpContent();
		
		const char* GetStorageTypeStr();
		const char* GetFilesystemTypeStr();
		const char* GetStorageDescriptionUtf8Str();
		const char* GetVolumeIdentifierUtf8Str();
		
};

#endif