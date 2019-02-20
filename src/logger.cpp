#include <cstdio>
#include "logger.h"


LogLevel Logger::level = Log_Debug; // Normal by default

void Logger::Log(int _level, const char* format, ...){
	
	if((int)level>= (int)_level){
		va_list ap;
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);
	}
	
}
