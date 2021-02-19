#include "WindowsService.h"

const short int TABLE_ENTRY_SIZE = 2;
const short int SERVICE_ARGUMENT = 1;

bool InteractWithServiceUsingCommandPrompt(const unsigned argc, const wchar_t* const argv[]);
bool InteractWithServiceUsingSCM(const SERVICE_TABLE_ENTRY* const service_table);

int wmain(unsigned argc, wchar_t* argv[])
{
	Service::set_service_name(L"AABA_Server");
	out.open("aout.txt");
	out << "STARTUEM\n";
	SERVICE_TABLE_ENTRY service_table[TABLE_ENTRY_SIZE]; // Contain entry for each service
	service_table[0].lpServiceName = const_cast<wchar_t*>(Service::get_instance().get_service_name().c_str());
	service_table[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(Service::get_instance().Main);

	service_table[1].lpServiceName = nullptr;
	service_table[1].lpServiceProc = nullptr;

	InteractWithServiceUsingSCM(service_table);
	//InteractWithServiceUsingCommandPrompt(argc, argv);
	/*if (!InteractWithServiceUsingCommandPrompt(argc, argv))
	{
		SERVICE_TABLE_ENTRY service_table[TABLE_ENTRY_SIZE]; // Contain entry for each service
		service_table[0].lpServiceName = const_cast<wchar_t*>(Service::get_instance().get_service_name().c_str());
		service_table[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(Service::get_instance().Main);

		service_table[1].lpServiceName = nullptr;
		service_table[1].lpServiceProc = nullptr;

		InteractWithServiceUsingSCM(service_table);
	}*/

	return 0;
}

bool InteractWithServiceUsingCommandPrompt(const unsigned argc, const wchar_t* const argv[])
{
	if (argc > 1)
	{
		if (!wcscmp(argv[SERVICE_ARGUMENT], INSTALL.data()))			return Service::get_instance().Install();
		else if (!wcscmp(argv[SERVICE_ARGUMENT], START.data()))			return Service::get_instance().Start();
		else if (!wcscmp(argv[SERVICE_ARGUMENT], STOP.data()))			return Service::get_instance().Stop();
		else if (!wcscmp(argv[SERVICE_ARGUMENT], RESTART.data()))		return Service::get_instance().Restart();
		else if (!wcscmp(argv[SERVICE_ARGUMENT], UNINSTALL.data()))		return Service::get_instance().Uninstall();
		else std::cerr << "Invalid input\n";
	}	
	return false;
}

bool InteractWithServiceUsingSCM(const SERVICE_TABLE_ENTRY* const service_table)
{
	out << "DISPATCHER\n";
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