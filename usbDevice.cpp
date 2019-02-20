#include <cstdio>
#include <cstring>

#include "usbDevice.h"
#include "logger.h"
#include "returnConstants.h"

bool UsbDevice::__usb_initialized = false;


UsbDevice::UsbDevice(){
	ctx = NULL;
	handle = NULL;
	
	debugCommunications = false;
	debugInitReleaseItems = false;
}

UsbDevice::~UsbDevice(){
	ReleaseDevice();
	CloseDevice();
	FinalizeUsb();
}

int UsbDevice::UsbRead(ByteVector* data,int size,int timeout){
	int nread = 0;
	
	unsigned int howMuchToRead = (size!=-1) ? size : data->capacity;
	
	int ret = libusb_bulk_transfer(handle, USB_ENDPOINT_IN, data->memory, howMuchToRead, &nread, timeout);

	data->size = (ret) ? 0 : nread;
	
	if(debugCommunications){
		Logger::Log(Log_Normal,"[UsbDevice] Asked to read:%d, Capacity: %d, Ret:%d, Received (%d bytes)\n<< ", howMuchToRead,data->capacity, ret, nread);
	}
	
	if(nread>0){
		if(debugCommunications){
			data->DumpContent();
		}
		return nread;
	}
	
	return (ret) ? -1 : 0;
}

int UsbDevice::UsbWrite(ByteVector* data){
	int nwritten = 0;
	
	if(debugCommunications){
		Logger::Log(Log_Normal,"[UsbDevice] Writing (%d bytes)\n>> ", data->size);
		data->DumpContent();
	}
	
	int ret = libusb_bulk_transfer(handle, USB_ENDPOINT_OUT, data->memory, data->size, &nwritten, USB_TIMEOUT);
	
	return (ret) ? -1 : nwritten;
}

int UsbDevice::InitializeUsb(){
	if(!__usb_initialized){
		libusb_init(&ctx);

#ifdef __HAIKU__
		//libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
		//libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
#endif

#ifdef __linux__
		//libusb_set_debug(ctx, 3);
#endif		
		if(debugInitReleaseItems){
			Logger::Log(Log_Debug,"[UsbDevice] Initialized usb succesfully\n");
		}
		__usb_initialized = true;
		return MUMTP_SUCCESS;
	}
	return MUMTP_SUCCESS;
}

int UsbDevice::OpenDevice(unsigned int vid, unsigned int pid,int interface, int alternate){
	
	handle = libusb_open_device_with_vid_pid(ctx,vid, pid);
	if (!handle) {
		return MUMTP_DEVICE_VIDPID_NOT_FOUND;
	}
	
	int r = 1;
	
	//Claim Interface 0 from the device
	r = libusb_claim_interface(handle, interface);
	if (r < 0) {
		return MUMTP_DEVICE_INTERFACE_CLAIM_FAILED;
	}
	
	/*r = libusb_set_interface_alt_setting(handle, interface, alternate);
	if (r < 0) {
		Logger::Log(Log_Debug,"[UsbDevice] Failed to set the interface %d to the alternate %d\n", interface, alternate);
		return false;
	}*/
	
	return MUMTP_SUCCESS;
}

bool UsbDevice::ReleaseDevice(){
	if(handle!=NULL){
		if(debugInitReleaseItems){
			Logger::Log(Log_Debug,"[UsbDevice] Releasing device interface\n");
		}
		libusb_release_interface (handle, 0);
		return true;
	}
	return true;
}

bool UsbDevice::CloseDevice(){
	if(handle!=NULL){
		if(debugInitReleaseItems){
			Logger::Log(Log_Debug,"[UsbDevice] Closing current device\n");
		}
		libusb_close(handle);
		handle = NULL;
		return true;
	}
	return true;
}


bool UsbDevice::FinalizeUsb(){
	if(__usb_initialized){
		if(debugInitReleaseItems){
			Logger::Log(Log_Debug,"[UsbDevice] Finalizing usb\n");
		}
		libusb_exit(NULL);
		__usb_initialized = false;
		return true;
	}
	return true;
}


bool UsbDevice::DumpDevices(){
	
	unsigned int VMWARE_VENDOR_ID = 0x0e0f;
	
	Logger::Log(Log_Normal,"[*] Scanning USB devices \n");
	
	if(InitializeUsb()==MUMTP_SUCCESS){
		int count = 0;
		libusb_device **list = NULL;
		count = libusb_get_device_list(ctx, &list);
		
		if(count>0){
			libusb_device *device;
			ByteVector description(260);
			ByteVector manufacturer(256);
			ByteVector endpoints(250);
			int rc = 0;
			
			for(int i = 0;i<count;++i){
				device = list[i];
				libusb_device_descriptor desc = {0};
				
				rc = libusb_get_device_descriptor(device, &desc);
				if(rc==0){
					
					if(desc.idVendor == VMWARE_VENDOR_ID){
						Logger::Log(Log_Normal,"[-] VMWARE Device detected, skipping it\n");
						continue;
					}

					libusb_device_handle *localHandle = NULL;
					rc = libusb_open(device, &localHandle);
					if(rc==LIBUSB_SUCCESS){
						bool manufacturerOk = GetDeviceManufacturer(localHandle, desc, &manufacturer);
						bool descriptionOk = GetDeviceDescription(localHandle, desc, &description);					
						
						if(!manufacturerOk || !descriptionOk){
							Logger::Log(Log_Normal,"[-] Failed to fetch manufacturer / description. Skipping device\n");
							continue;
						}
						
						Logger::Log(Log_Normal,"[-] %s-%s [vID %04X, pID %04X]. Config count:%d\n", manufacturer.memory, description.memory, desc.idVendor, desc.idProduct, desc.bNumConfigurations);
						
						bool endpointsOk = GetDeviceEndpoints(device, desc, &endpoints);
						if(!endpointsOk){
							//TODO: doing something here?
						}
					}
					if(localHandle){
						libusb_close(localHandle);
					}

				}
			}			
		}else{
			Logger::Log(Log_Normal,"[UsbDevice] No USB devices found\n");
		}
		
		libusb_free_device_list(list, count);
		return true;
	}
	
	Logger::Log(Log_Normal,"[UsbDevice] Failed to initialize usb, no device listing\n");
	return true;
}

bool UsbDevice::GetDeviceManufacturer(libusb_device_handle* dev_handle, libusb_device_descriptor& desc, ByteVector* output){
	int rc = 0;
	output->memory[0] = 0x00;
	if (desc.iManufacturer) {
		rc = libusb_get_string_descriptor_ascii(dev_handle, desc.iManufacturer, output->memory, output->capacity);
	}
	return (rc>0);
}

bool UsbDevice::GetDeviceDescription(libusb_device_handle* dev_handle, libusb_device_descriptor& desc, ByteVector* output){
	int rc = 0;
	output->memory[0] = 0x00;
	if (desc.iProduct) {
		rc = libusb_get_string_descriptor_ascii(dev_handle, desc.iProduct, output->memory, output->capacity);
	}
	return (rc>0);
}

bool UsbDevice::GetDeviceEndpoints(libusb_device* device, libusb_device_descriptor& desc, ByteVector* output){
	
	int rc = 0;
	
	for (int i = 0; i < desc.bNumConfigurations; i++) {
		struct libusb_config_descriptor *config;
		rc = libusb_get_config_descriptor(device, i, &config);
		if (LIBUSB_SUCCESS != rc) {
			continue;
		}
		
		for(int j = 0;j<config->bNumInterfaces;++j){
			libusb_interface interface = config->interface[j];
			//Logger::Log(Log_Normal,"[UsbDevice] Interface found: %d. Alternative settings: %d\n", j, interface.num_altsetting);
			
			for(int k = 0;k<interface.num_altsetting;k++){
				const libusb_interface_descriptor* interfaceDescriptor = &(interface.altsetting[k]);
				
				if(interfaceDescriptor->bNumEndpoints<3){
					continue;
				}
				
				int totalBulkInEP = 0;
				int totalBulkOutEP = 0;
				int totalInterruptInEP = 0;
				unsigned int bulkInAddr = 0xFF;
				unsigned int bulkOutAddr = 0xFF;
				unsigned int intAddr = 0xFF;
				
				for(int endp = 0;endp<interfaceDescriptor->bNumEndpoints;endp++){
					const libusb_endpoint_descriptor *endpoint = &(interfaceDescriptor->endpoint[endp]);
					//Logger::Log(Log_Normal,"[UsbDevice] bEndpointAddress: %08X\n", endpoint->bEndpointAddress);
					//Logger::Log(Log_Normal,"[UsbDevice] bmAttributes: %08X\n", endpoint->bmAttributes);

					int direction = endpoint->bEndpointAddress & 0x80;
					int value = endpoint->bmAttributes & 0x03;
					
					//Logger::Log(Log_Normal,"[UsbDevice] direction: %08X\n", direction);
					
					switch(value){
						case LIBUSB_TRANSFER_TYPE_BULK :
							if(direction == LIBUSB_ENDPOINT_IN){
								totalBulkInEP++;
								bulkInAddr = endpoint->bEndpointAddress;
							}else{
								totalBulkOutEP++;
								bulkOutAddr = endpoint->bEndpointAddress;
							}
							break;
						case LIBUSB_TRANSFER_TYPE_INTERRUPT :
							if(direction == LIBUSB_ENDPOINT_IN){
								totalInterruptInEP++;
								intAddr = endpoint->bEndpointAddress;
							}
							break;
					}
				}
				
				
				//Logger::Log(Log_Normal,"[UsbDevice] Endpoint resume: %d %d %d\n", totalBulkInEP ,  totalBulkOutEP ,totalInterruptInEP);
				if(totalBulkInEP >= 1 && totalBulkOutEP >= 1 &&totalInterruptInEP>=1){
					Logger::Log(Log_Normal,"\t>>> MTP alike values: Configuration: %d, Interface: %d, Alternate: %d, EndP-In: %02X, EndP-Out: %02X, EndP-Int: %02X\n", i,j,k, bulkInAddr, bulkOutAddr, intAddr);
				}

				/*Logger::Log(Log_Normal,"[UsbDevice] bInterfaceNumber: %d. \n", interfaceDescriptor->bInterfaceNumber);
				Logger::Log(Log_Normal,"[UsbDevice] iInterface: %d. \n", interfaceDescriptor->iInterface);
				Logger::Log(Log_Normal,"[UsbDevice] bNumEndpoints: %d. \n", interfaceDescriptor->bNumEndpoints);
				*/
			}			
		}

		libusb_free_config_descriptor(config);
	}
	return true;
}
