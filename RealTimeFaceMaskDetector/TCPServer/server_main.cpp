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
	LOG_MSG << "wmain: start";
	
	bool result = ChooseServerBootOptionAndStart(argc, argv);

	LOG_MSG << "wmain: finish";
	return result ? 0 : -1;
}

bool ChooseServerBootOptionAndStart(const unsigned argc, const wchar_t* const argv[])
{
	bool ret_val = false;
	if(argc > 1)
	{
		LOG_MSG << "ChooseServerBootOptionAndStart: Starting server as service...";
		ret_val = RunServerUnderService(argv);
	}
	else
	{
		LOG_MSG << "ChooseServerBootOptionAndStart: Starting server as console app...";
		ret_val = RunServerItself();
	}
	return ret_val;
}

bool RunServerUnderService(const wchar_t* const argv[])
{
	ServiceStarter service;

	if (!service.SetServiceName(argv[SERVICE_ARGUMENT]))
	{
		LOG_ERROR << "RunServerUnderService: Invalid service name";
		return false;
	}

	SERVICE_TABLE_ENTRY service_table[TABLE_ENTRY_SIZE];
	CreateServiceTableEntry(service, &service_table);

	return InteractWithServiceUsingSCM(service_table);
}
bool InteractWithServiceUsingSCM(const SERVICE_TABLE_ENTRY* const service_table)
{
	LOG_MSG << "InteractWithServiceUsingSCM: calling StartServiceCtrlDispatcher...";
	StartServiceCtrlDispatcher(service_table); // Connect main thread of service procces with the SCM
	int error = GetLastError();
	if (error)
	{
		LOG_ERROR << "StartServiceCtrlDispatcher ERROR " << error;
		return false;
	}
	LOG_MSG << "StartServiceControlDispatcher succeeded :)";
	return true;
}

void CreateServiceTableEntry(ServiceStarter& service, SERVICE_TABLE_ENTRY(*entry_table)[2])
{
	LOG_MSG << "CreateServiceTableEntry: Creating table";
	(*entry_table)[0].lpServiceName = const_cast<wchar_t*>(service.GetServiceName().c_str());
	(*entry_table)[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(service.ServiceMain);

	(*entry_table)[1].lpServiceName = nullptr;
	(*entry_table)[1].lpServiceProc = nullptr;
}