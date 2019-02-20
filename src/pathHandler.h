#ifndef PATHHANDLER_H
#define PATHHANDLER_H

#include <stdio.h>
#include <vector>

#include <iostream>

class PathHandler{

public:

	PathHandler();
	~PathHandler();
	
	static std::vector<std::string> SplitPath(const char* str, int pathLength);
	static std::string GetLastPart(const char* str, int pathLength);
};

#endif // PATHHANDLER_H
