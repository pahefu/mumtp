#ifndef MTPCOMMANDS__H
#define MTPCOMMANDS__H

enum MTP_COMMANDS {

	MTP_GetDeviceInfo = 0x1001,
	MTP_OpenSession = 0x1002,
	MTP_CloseSession = 0x1003,
	MTP_GetStorageIDs = 0x1004,
	MTP_GetStorageInfo = 0x1005,
	MTP_GetNumObjects = 0x1006,
	MTP_GetObjectHandles = 0x1007,
	MTP_GetObjectInfo = 0x1008,
	MTP_GetObject = 0x1009,
	/**/
	MTP_GetDevicePropValue = 0x1015,
	
};

enum MTP_ANSWERS {
	MTP_ANS_OK = 0x2001,
	MTP_ANS_SESSION_NOT_OPEN = 0x2003,
	/**/
	MTP_ANS_DEVICEPROP_NOT_SUPPORTED = 0x200A,
	/**/
	MTP_ANS_SESSION_ALREADY_OPEN = 0x201E
};

#endif
