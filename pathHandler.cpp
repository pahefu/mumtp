#include "pathHandler.h"
#include <cstring>

PathHandler::PathHandler()
{
}

PathHandler::~PathHandler()
{
}

std::vector<std::string> PathHandler::SplitPath(const char* str, int pathLength){
	std::vector<std::string> result;
	std::string temp = "";
	
	int str_len = pathLength;
	for(int i = 0;i< str_len;++i){
		if(str[i]=='/'){
			
			if(temp==""){
				temp = "/"; // corner case
			}
			
			result.push_back(temp);
			temp = "";
		}else{
			temp+=str[i];
		}
	}
	
	if(temp!=""){
		result.push_back(temp);
	}

	return result;
}

std::string PathHandler::GetLastPart(const char* str, int pathLength){
	std::vector<std::string> results = PathHandler::SplitPath(str,pathLength);
	std::string result = "";
	if(results.size()>=1){
		result = results.at(results.size()-1);
	}
	return result;
}
