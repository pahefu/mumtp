#ifndef BYTEVECTOR__H
#define BYTEVECTOR__H

#include <iostream>
#include <inttypes.h>

typedef unsigned char byte;

class ByteVector{
	
private:
	void PopBytes(unsigned int n);
	byte* ReserveMemory(unsigned int size);
	
public:
	byte* memory;
	unsigned int size;
	unsigned int capacity;
	unsigned int growStep;
	
	ByteVector(unsigned int _capacity = 32, unsigned int _growStep = 16);
	~ByteVector();
	
	void Empty();
	void DisposeMemory();
	
	void GrowUntilFit(unsigned int wantedSize);
	
	void PushByte(unsigned char b);
	void PushUint32(unsigned int n);
	void PushUint16(unsigned short n);
	
	byte PopByte();
	
	uint64_t PopUint64();
	unsigned int PopUint32();
	unsigned short PopUint16();
	
	bool PopMTPUnicodeString(ByteVector* output, bool safeCharsOnly);
	
	void PushVectorData(ByteVector& other, int maxSize = -1);
	void PushVectorData(ByteVector* other, int maxSize = -1);
	
	void DoTests();
	
	void DumpContent();
	
	bool EqualsCharStr(const char* str, unsigned int strLen);
	
};

#endif