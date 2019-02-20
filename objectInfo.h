#ifndef OBJECTINFO__H
#define OBJECTINFO__H

#include <byteVector.h>

class ObjectInfo{

public:
		bool valid;
		unsigned int objectHandle;
		
		unsigned int storageId;
		unsigned short objectFormat;
		unsigned short protectionStatus;
		unsigned int objectCompressedSize;
		
		unsigned short thumbFormat;
		unsigned int thumbCompressedSize;
		unsigned int thumbPixWidth;
		unsigned int thumbPixHeight;
		
		unsigned int imagePixWidth;
		unsigned int imagePixHeight;
		unsigned int imageBitDepth;
		
		unsigned int parentObject;
		unsigned short associationType;
		unsigned int associationDescription;
		unsigned int sequenceNumber;
		
		ByteVector* filename;
		ByteVector* dateCreated;
		ByteVector* dateModified;
		ByteVector* keywords;
		
		ObjectInfo();
		ObjectInfo(unsigned int handlerId);
		~ObjectInfo();

		void FromVector(ByteVector& v);
		void FromVector(ByteVector* v);
		void DumpContent();
		
		bool FileNameEquals(const char* str, int strLen);
		
		bool CloneFromOther(ObjectInfo* other);
		bool CloneFromOther(ObjectInfo& other);
};

#endif