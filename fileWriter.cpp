#include <cstring>
#include <cstdlib>

#include "fileWriter.h"
#include "logger.h"


FileWriter::FileWriter(){
	fileOpen = false;
	totalWritten = 0;
}

FileWriter::~FileWriter(){
	if(fileOpen){
		CloseFile();
	}
}

void FileWriter::SetFileName(const char* fileName, int fileNameLength){
	//TODO: ?
}

bool FileWriter::OpenFile(const char* fileName, int fileNameLength, const char* args){
	if(!fileOpen){
		fd = fopen(fileName, args);
		totalWritten = 0;
		fileOpen = (fd>0);
		return fileOpen;
	}
	return false;
}

bool FileWriter::WriteToFile(unsigned char* data, int dataLength){
	if(fileOpen){
		int n = fwrite(data, 1,dataLength, fd);
		if(n>0){
			totalWritten+=n;
		}	
		return (n>0);
	}
	return false;
}

bool FileWriter::CloseFile(){
	if(fileOpen){
		fclose(fd);
		fileOpen = false;
	}
	return true;
}
