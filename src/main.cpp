#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

#include <returnConstants.h>
#include <mtpOperator.h>
#include <logger.h>

// External dependencies, header-only libraries from github
#include "argh.h"
#include "INIReader.h"

// The mtp operator global object for this main example
MtpOperator oper;

int test_main(){
	
	int result = MUMTP_SUCCESS;
	
	result = oper.GetDeviceInfo();
	if(result!=MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"[Error] Failed to get device info\n");
		return 1;
	}

	if(oper.SyncDeviceFriendlyName() == MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Device is: %s\n", oper.device.deviceFriendlyName.memory);
	}
	
	if(oper.SyncBatery() == MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Battery is: %d\n", oper.device.batteryLevel);
	}
	
	if(oper.SyncStorages() == MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Storage: Capacity: %dMB Free space: %dMB\n", oper.device.storage.humanMaxCapacity, oper.device.storage.humanFreeSpace);
	}
	
	if(oper.SyncStorageObjectCount() == MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Total objects in storage: %08X\n", oper.device.storage.totalObjectsInStorage);
	}
	return 0;
}

int main_scan(){
	
	oper.device.DumpDevices();
	
	return 0;
}

int main_get_handle(std::string handleStr){
	
	if(oper.SyncStorages()!=MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Cannot Synchronize storages. Is the phone set up?\n");
		return 1;
	}
	
	unsigned int handleId = std::stoul(handleStr, nullptr, 16);
	Logger::Log(Log_Normal,"Trying to download file by handle: '%08X'\n", handleId);
	
	int result = oper.DownloadFile(handleId,NULL);
	
	switch(result){
		case MUMTP_SUCCESS: 
			Logger::Log(Log_Normal,"Succesfully downloaded file by handle: '%08X'\n", handleId);
			return 0;
		break;
		
		default:
			Logger::Log(Log_Normal,"[Error] Failed to download file by handle: '%08X'. Error code: %d\n", handleId, result);
			return 1;
		break;
	}

}

int main_get(std::string pathToGet){
	
	if(oper.SyncStorages()!=MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Cannot Synchronize storages. Is the phone set up?\n");
		return 1;
	}
	
	Logger::Log(Log_Normal,"Downloading file: '%s'\n", pathToGet.c_str());
	int result = oper.DownloadFile(pathToGet.c_str(), pathToGet.length());
	
	switch(result){
		case MUMTP_SUCCESS: 
			Logger::Log(Log_Normal,"Succesfully downloaded file by name: %s'\n", pathToGet.c_str());
			return 0;
		break;
		
		default:
			Logger::Log(Log_Normal,"[Error] Failed to download file by name: '%s'. Error code: %d\n", pathToGet.c_str(), result);
			return 1;
		break;
	}
	
	return 0;
}

int main_ls(std::string pathToShow){
	
	if(oper.SyncStorages()!=MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Cannot Synchronize storages. Is the phone set up?\n");
		return 1;
	}
	
	if(pathToShow==""){
		pathToShow = "/";
	}
	
	if(oper.CdToPath(pathToShow.c_str(), pathToShow.length())!=MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"Cannot open the path: '%s'\n", pathToShow.c_str());
		return 1;
	}
	
	
	std::vector<ObjectInfo*> objectsInfo;
	int result = oper.GetPathObjects(objectsInfo, oper.currentObject.objectHandle);
	
	if(result!=MUMTP_SUCCESS){
		Logger::Log(Log_Normal,"[Error] couldnt list children for the selected handle\n");
		return 1;
	}
	
	Logger::Log(Log_Normal,">> '%s'\n", pathToShow.c_str());
	Logger::Log(Log_Normal,">> Total objects: %d\n", objectsInfo.size());
	Logger::Log(Log_Normal,"-------------------------------\n");

	
	for(unsigned int i=0;i<objectsInfo.size();++i){
		ObjectInfo* infoPtr = objectsInfo.at(i);	
		Logger::Log(Log_Debug,"%08X\t", infoPtr->objectHandle);
		
		for(unsigned int k = 0;k<infoPtr->dateModified->size;++k){
			Logger::Log(Log_Normal,"%c", infoPtr->dateModified->memory[k]);
		}
		
		Logger::Log(Log_Debug,"\t");
		
		for(unsigned int k = 0;k<infoPtr->filename->size;k+=2){
			Logger::Log(Log_Normal,"%c", infoPtr->filename->memory[k]);
		}

		Logger::Log(Log_Debug,"\n");
		delete infoPtr;
	}
	
	
	
	return 0;
}

int main_usage(){
	Logger::Log(Log_Debug,"Usage: \n\n");
	Logger::Log(Log_Debug,"--scan\tLists MTP alike device configurations in USB devices\n");
	Logger::Log(Log_Debug,"--ls\tList a directory with the device\n");
	Logger::Log(Log_Debug,"--get\tDownloads a file from the device by path\n");
	Logger::Log(Log_Debug,"--geth\tDownloads a file from the device by handle id (hex)\n");
	Logger::Log(Log_Debug,"--createconf\tCreates a dummy config file\n");
	
	return 0;
}

bool createConfig(){
	bool result = false;
	
	std::ofstream outfile ("./mumtp.ini");
	outfile << "[device]" << std::endl;
	outfile << "vendorID = 0x00" << std::endl;
	outfile << "productID = 0x00" << std::endl;
	outfile << "endpointIN = 0x00" << std::endl;
	outfile << "endpointOUT = 0x00" << std::endl;
	outfile.close();
	
	return result;
}

bool loadConfig(){
	std::vector<std::string> osPaths;
	
	osPaths.push_back("./mumtp.ini"); // The local and only one file
	osPaths.push_back("~/mumtp.ini");
	osPaths.push_back("~/config/settings/mumtp/mumtp.ini");
	osPaths.push_back("~/config/mumtp.ini");
	osPaths.push_back("~/.config/mumtp.ini");
	osPaths.push_back("/boot/home/mumtp.ini");
	osPaths.push_back("/boot/home/config/settings/mumtp/mumtp.ini");

	INIReader reader;
	bool found = false;
	std::string invalidValue = "INVALID";
	
	for(unsigned int i = 0;i<osPaths.size();++i){
		
//		Logger::Log(Log_Debug,"Testing file: %s\n", osPaths.at(i).c_str());

		FILE* fd = fopen(osPaths.at(i).c_str(),"rb");
		if(!fd){
			continue;
		}
		INIReader reader(fd);

		int parseResult = reader.ParseError();
		fclose(fd);

		if(parseResult==0){
			
			std::string vid = reader.Get("device", "vendorID", invalidValue);
			if(vid==invalidValue){
				continue;
			}
			
			std::string pid = reader.Get("device", "productID", invalidValue);
			if(pid==invalidValue){
				continue;
			}
			
			std::string endpIn = reader.Get("device", "endpointIN", invalidValue);
			if(endpIn==invalidValue){
				continue;
			}
			
			std::string endpOut = reader.Get("device", "endpointOUT", invalidValue);
			if(endpOut==invalidValue){
				continue;
			}
			
			Logger::Log(Log_Debug,"Using config file: %s\n", osPaths.at(i).c_str());
			
			oper.device.VID = std::stoul(vid, nullptr, 16);
			oper.device.PID = std::stoul(pid, nullptr, 16);
			oper.device.USB_ENDPOINT_IN = std::stoul(endpIn, nullptr, 16);
			oper.device.USB_ENDPOINT_OUT = std::stoul(endpOut, nullptr, 16);
			
			found = true;
			break;
			
		}
	}
	return found;
}

int main(int argc, char **argv){

	if(argc==1){
		return main_usage();
	}

	// ==> Process the commands from the user
	argh::parser cmdl;
	cmdl.add_params({"--ls","--get","--geth"});
	cmdl.parse(argc, argv);

	if(oper.device.InitializeUsb()!=MUMTP_SUCCESS){
		return 1;
	}

	// Non device related,  scan options

	if(cmdl["scan"]){
		return main_scan();
	}
	
	if(cmdl["createconf"]){
		createConfig();
		return 0;
	}

	// Device related options
	
	if(!loadConfig()){
		Logger::Log(Log_Debug,"Cannot find any valid config file for the device values. Giving up\n");
		Logger::Log(Log_Debug,"Try setting it with the values from a scan output\n");
		return 1;
	}

	// This may need to be set too in the file, but there is no backend code to change them yet
	oper.device.configurationId = 0;
	oper.device.interfaceId = 0;
	oper.device.debugCommunications = false;
	
	int openRes = oper.device.OpenDevice(oper.device.VID,oper.device.PID,oper.device.configurationId);
	switch(openRes){
		case MUMTP_DEVICE_VIDPID_NOT_FOUND: 
			Logger::Log(Log_Debug,"[Error] Failed to open a device with values vendID %04X: prID %04X\n", oper.device.VID, oper.device.PID);
			return 1;
		break;
		
		case MUMTP_DEVICE_INTERFACE_CLAIM_FAILED: 
			Logger::Log(Log_Debug,"[Error] Failed to claim the device interface %d\n", 0);
			return 1;
		break;
	}

	if(cmdl("ls") || cmdl["ls"]){	
		return main_ls(cmdl("ls").str());
	}
	
	if(cmdl("get")){
		return main_get(cmdl("get").str());
	}
	
	if(cmdl("geth")){
		return main_get_handle(cmdl("geth").str());
	}

	Logger::Log(Log_Debug, "Command arguments unknown\n");

	return 0;
	
}
