#include <iostream>
#include "ServiceStarter.h"
const short int TABLE_ENTRY_SIZE = 2;
const short int SERVICE_NAME_ARGUMENT = 1;
const short int WORKING_DIRECTORY = 2;
std::string LOG_FILENAME = R"(ServerLogsConfig.conf)";

bool ChooseServerBootOptionAndStart(const unsigned argc, const wchar_t* const argv[]);
bool RunServerUnderService(const wchar_t* const argv[]);
bool InteractWithServiceUsingSCM(const SERVICE_TABLE_ENTRY* const service_table);
void CreateServiceTableEntry(ServiceStarter& service, SERVICE_TABLE_ENTRY(*entry_table)[2]);


int wmain(unsigned argc,  wchar_t* argv[])
{
	bool result = ChooseServerBootOptionAndStart(argc, argv);

	LOG_MSG << "wmain: finish";
	return result ? 0 : -1;
}

bool ChooseServerBootOptionAndStart(const unsigned argc, const wchar_t* const argv[])
{
	bool ret_val = false;
	//Logger::SetConfiguration(LOG_FILENAME);
	if(argc > 2)
	{
		if(!SetCurrentDirectoryW(argv[WORKING_DIRECTORY]))
	{
			LOG_WARNING << "SetCurrentDirectory: ERROR: " << GetLastError();
			return false;
		}
		Logger::SetConfiguration(LOG_FILENAME);
		LOG_MSG << "ChooseServerBootOptionAndStart: Starting server as service...";
		ret_val = RunServerUnderService(argv);
	}
	else
	{
		Logger::SetConfiguration(LOG_FILENAME);
		LOG_MSG << "ChooseServerBootOptionAndStart: Starting server as console app...";
		ret_val = RunServerItself();
	}
	return ret_val;
}

bool RunServerUnderService(const wchar_t* const argv[])
{
	ServiceStarter service;

	if (!service.SetServiceName(argv[SERVICE_NAME_ARGUMENT]))
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
	(*entry_table)[0].lpServiceName = (LPSTR) service.GetServiceName().c_str();
	(*entry_table)[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(service.ServiceMain);

	(*entry_table)[1].lpServiceName = nullptr;
	(*entry_table)[1].lpServiceProc = nullptr;
}