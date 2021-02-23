#include <iostream>
#include "ServiceStarter.h"
const short int TABLE_ENTRY_SIZE = 2;
const short int SERVICE_ARGUMENT = 1;

bool InteractWithServiceUsingSCM(const SERVICE_TABLE_ENTRY* const service_table);
void CreateServiceTableEntry(ServiceStarter& service, SERVICE_TABLE_ENTRY(*entry_table)[2]);

int wmain(unsigned argc,  wchar_t* argv[])
{
	os.open("D:\\Programing\\Projects\\Real-Time-Face-Mask-Detector-Server\\RealTimeFaceMaskDetector\\x64\\Debug\\AAAout.txt",
		std::ios_base::app);
	os << "\nMAIN WORKS\n";
	
	ServiceStarter service;

	if(!service.SetServiceName(argv[SERVICE_ARGUMENT]))
	{
		os << "Invalid name of service\n";
		return 1;
	}

	SERVICE_TABLE_ENTRY service_table[TABLE_ENTRY_SIZE];
	CreateServiceTableEntry(service, &service_table);

	/*service_table[0].lpServiceName = const_cast<wchar_t*>(service.GetServiceName().c_str());
	service_table[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(service.ServiceMain);

	service_table[1].lpServiceName = nullptr;
	service_table[1].lpServiceProc = nullptr;*/

	InteractWithServiceUsingSCM(service_table);
	os.close();
	return 0;
}

bool InteractWithServiceUsingSCM(const SERVICE_TABLE_ENTRY* const service_table)
{
	os << "Dispatcher\n";
	StartServiceCtrlDispatcher(service_table); // Connect main thread of service procces with the SCM
	int error = GetLastError();
	if (error)
	{
		os << "StartServiceCtrlDispatcher ERROR " << error;
		return false;
	}
	os << "StartServiceControlDispatcher succeeded :)";
	return true;
}

void CreateServiceTableEntry(ServiceStarter& service, SERVICE_TABLE_ENTRY(*entry_table)[2])
{
	(*entry_table)[0].lpServiceName = const_cast<wchar_t*>(service.GetServiceName().c_str());
	(*entry_table)[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(service.ServiceMain);

	(*entry_table)[1].lpServiceName = nullptr;
	(*entry_table)[1].lpServiceProc = nullptr;
}