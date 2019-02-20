#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <cstdio>

class FileWriter{

public:

	unsigned char fileName[512];
	bool fileOpen;
	unsigned int totalWritten;
	FILE* fd;

	FileWriter();
	~FileWriter();
	
	void SetFileName(const char* fileName, int fileNameLength);
	bool OpenFile(const char* fileName, int fileNameLength, const char* args);
	bool WriteToFile(unsigned char* data, int dataLength);
	bool CloseFile();

};

#endif // FILEWRITER_H
