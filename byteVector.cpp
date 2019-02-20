#include <cstring>
#include <math.h> 
#include <assert.h>

#include "byteVector.h"
#include "logger.h"


ByteVector::ByteVector(unsigned int _capacity, unsigned int _growStep){
	this->size = 0;
	this->capacity = _capacity;
	this->growStep = _growStep;
	memory = ReserveMemory(this->capacity);
}

ByteVector::~ByteVector(){
	DisposeMemory();
}

void ByteVector::Empty(){
	memset(memory, 0x00, this->size); // Do a clean, Jeeves
	this->size = 0;
	
}

void ByteVector::DisposeMemory(){
	if(this->memory!=NULL){
		delete[] memory;
		memory = NULL;
	}
}

byte* ByteVector::ReserveMemory(unsigned int _size){
	byte* newMemory = new byte[_size];
	memset(newMemory, 0x00, _size);
	return newMemory;
}

void ByteVector::GrowUntilFit(unsigned int wantedSize){
	
	if(capacity>wantedSize){
		return;
	}
	
	//Logger::Log(Log_Debug,"[ByteVector] Growing to a new size: %d\n", wantedSize);
	//int newCapacity = capacity+(this->growStep * ((wantedSize +this->growStep - this->capacity) / this->growStep));
	
	int newCapacity = ((ceil(((wantedSize*1.0) - this->capacity) / this->growStep))*this->growStep)+this->capacity;
	
	
	//Logger::Log(Log_Debug,"[ByteVector] New calculated capacity is: %d\n", newCapacity);
	
	byte* newMemory = ReserveMemory(newCapacity);

	memcpy(newMemory,memory, size);
	delete[] memory;
	
	memory = newMemory;
	
	this->capacity = newCapacity;
}

void ByteVector::PopBytes(unsigned int n){
	if(n>size){
		return;
	}
	
	for(unsigned int i = n;i<size;++i){
		memory[i-n] = memory[i];
	}
	
	size-=n;
	
}


void ByteVector::PushUint32(unsigned int num){

	GrowUntilFit(size+4);
	
	byte c = (num&0x000000ff);
	this->memory[size++] = c;
	c = (num&0x0000ff00)>>8;
	this->memory[size++] = c;
	c = (num&0x00ff0000)>>16;
	this->memory[size++] = c;
	c = (num&0xff000000)>>24;
	this->memory[size++] = c;

}

void ByteVector::PushUint16(unsigned short num){
	GrowUntilFit(size+2);
	
	byte c = (num&0x00ff);
	this->memory[size++] = c;
	c = (num&0xff00)>>8;
	this->memory[size++] = c;
}

void ByteVector::PushByte(unsigned char b){
	GrowUntilFit(size+1);
	this->memory[size++] = b;
}


byte ByteVector::PopByte(){
	if(size<1){
		return 0;
	}
	
	byte c = this->memory[0];
	PopBytes(1);
	return c;
}



uint64_t ByteVector::PopUint64(){
	uint64_t result = 0;
	if(size<8){
		// TODO: should trigger one exception... ?
		return 0;
	}
	
	byte c = this->memory[0];
	result+= (c & 0x000000ff);
	c = this->memory[1];
	result+= ((uint64_t)(c & 0x000000ff))<<8;
	c = this->memory[2];
	result+= ((uint64_t)(c & 0x000000ff))<<16;
	c = this->memory[3];
	result+= ((uint64_t)(c & 0x000000ff))<<24;
	c = this->memory[4];
	result+= ((uint64_t)(c & 0x000000ff))<<32;
	c = this->memory[5];
	result+= ((uint64_t)(c & 0x000000ff))<<40;
	c = this->memory[6];
	result+= ((uint64_t)(c & 0x000000ff))<<48;
	c = this->memory[7];
	result+= ((uint64_t)(c & 0x000000ff))<<56;
	 
	PopBytes(8);
	
	
	
	return result;

}



unsigned int ByteVector::PopUint32(){
	unsigned int result = 0;
	
	if(size<4){
		// TODO: should trigger one exception... ?
		return 0;
	}
	
	byte c = this->memory[0];
	result+= (c & 0x000000ff);
	c = this->memory[1];
	result+= (c & 0x000000ff)<<8;
	c = this->memory[2];
	result+= (c & 0x000000ff)<<16;
	c = this->memory[3];
	result+= (c & 0x000000ff)<<24;

	PopBytes(4);
	
	return result;
}

unsigned short ByteVector::PopUint16(){

	unsigned short result = 0;
	
	if(size<2){
		//TODO: should trigger one exception??
		return 0;
	}
	
	byte c = this->memory[0];
	result+= (c & 0x000000ff);
	c = this->memory[1];
	result+= (c & 0x000000ff)<<8;
	
	PopBytes(2);
	
	return result;

}

void ByteVector::DoTests(){
	unsigned int source = 0x11223344;
	unsigned int destination = 0x00;

	unsigned short sourceSmall = 0x1122;
	unsigned short destinationSmall = 0x00;

	PushUint16(sourceSmall);
	assert(size==2);

	PushUint32(source);
	assert(size==6);

	destinationSmall = PopUint16();
	assert(sourceSmall==destinationSmall);
	assert(size==4);

	destination = PopUint32();
	assert(source==destination);
	assert(size==0);

}

void ByteVector::PushVectorData(ByteVector& other, int maxSize){
	PushVectorData(&other, maxSize);
}

void ByteVector::PushVectorData(ByteVector* other, int maxSize){
	unsigned int localSize = (maxSize!=-1) ? maxSize : other->size;
	
	GrowUntilFit(this->size+localSize);
	
	memcpy(this->memory+size, other->memory, localSize);
	this->size+= localSize;
}

void ByteVector::DumpContent(){
	for(unsigned int i = 0;i<this->size;++i){
		Logger::Log(Log_Normal, "%02X", this->memory[i]);
	}
	Logger::Log(Log_Normal, "\n");
}

bool ByteVector::PopMTPUnicodeString(ByteVector* output, bool safeCharsOnly){
	output->Empty();
	
	unsigned int totalChars = this->PopByte();
	if(!safeCharsOnly){
		output->PushVectorData(this, totalChars*2);
		this->PopBytes(totalChars*2);
	}
	else{
		for(unsigned int i = 0;i<totalChars;i++){
			output->PushByte(this->PopByte());
			this->PopByte();
		}
	}
	return true;
}

bool ByteVector::EqualsCharStr(const char* str, unsigned int strLen){

	unsigned int localSize = this->size;
	
	if(str[strLen-1]!=0x00 && memory[size-1]==0x00){ // We are a NULL-T cstring and the Str is not
		localSize--;
		if(memory[size-2]==0x00){ // Not only cstring, but also utf-8
			localSize--;
		}
	}
	
	if(localSize!=strLen && localSize!=(strLen*2)){
		return false; // Non utf8, non raw str len match
	}
	
	if(localSize==strLen){ // Length matches perfectly
		return (memcmp(str, this->memory, localSize)==0);
	}else{	// Local size is utf8 while str is not
		
		int index = 0;
		for(unsigned int i = 0;i<localSize;i+=2){
			if(str[index]!=this->memory[i]){
				return false;
			}
			index++;
		}
		return true;		
	}
	return false;
}
