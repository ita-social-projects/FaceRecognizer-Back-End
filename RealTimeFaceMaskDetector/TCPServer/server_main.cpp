#include <iostream>
#include "ServiceStarter.h"
const short int TABLE_ENTRY_SIZE = 2;
const short int SERVICE_ARGUMENT = 1;

bool ChooseServerBootOptionAndStart(const unsigned argc, const wchar_t* const argv[]);
bool RunServerUnderService(const wchar_t* const argv[]);
bool InteractWithServiceUsingSCM(const SERVICE_TABLE_ENTRY* const service_table);
void CreateServiceTableEntry(ServiceStarter& service, SERVICE_TABLE_ENTRY(*entry_table)[2]);


int wmain(unsigned argc,  wchar_t* argv[])
{
	os.open("D:\\Programing\\Projects\\Real-Time-Face-Mask-Detector-Server\\RealTimeFaceMaskDetector\\x64\\Debug\\AAAout.txt",
		std::ios_base::app);
	os << "\nMAIN WORKS\n";
	
	bool result = ChooseServerBootOptionAndStart(argc, argv);
	os.close();

	return result ? 0 : -1;
}

bool ChooseServerBootOptionAndStart(const unsigned argc, const wchar_t* const argv[])
{
	bool ret_val = false;
	if(argc > 1)
	{
		ret_val = RunServerUnderService(argv);
	}
	else
	{
		ret_val = RunServerItself();
	}
	return ret_val;
}

bool RunServerUnderService(const wchar_t* const argv[])
{
	ServiceStarter service;

	if (!service.SetServiceName(argv[SERVICE_ARGUMENT]))
	{
		os << "Invalid name of service\n";
		return false;
	}

	SERVICE_TABLE_ENTRY service_table[TABLE_ENTRY_SIZE];
	CreateServiceTableEntry(service, &service_table);

	return InteractWithServiceUsingSCM(service_table);
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