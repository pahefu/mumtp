#ifndef LOGGER__H
#define LOGGER__H

#include <iostream>
#include<stdarg.h>


enum LogLevel{
	Log_None = 0,
	Log_Normal= 1,
	Log_Warning = 2,
	Log_Debug = 3
};

class Logger{
	
public:
	static LogLevel level;
	static void Log(int _level, const char* format, ...);
	
};

#endif