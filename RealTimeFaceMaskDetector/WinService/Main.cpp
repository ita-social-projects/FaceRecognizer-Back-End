#include "WindowsService.h"
#define CONFIG_FILE "config.xml"
constexpr short SERVICE_CONTROL_OPTION = 1;

bool InteractWithServiceUsingCommandPrompt(const unsigned argc, const wchar_t* const argv[]);

int wmain(unsigned argc, wchar_t* argv[])
{
	std::unique_ptr<ConfigReader>parser = std::make_unique<XMLParser>(CONFIG_FILE);
	std::string name= parser->GetParam("Service", "name");

	std::wstring service_name(name.begin(), name.end());

	LOG_MSG << "WinService controller: wmain";
	Service::set_service_name(service_name);

	InteractWithServiceUsingCommandPrompt(argc, argv);
	return 0;
}

bool InteractWithServiceUsingCommandPrompt(const unsigned argc, const wchar_t* const argv[])
{
	if (argc > 1)
	{
		if (!wcscmp(argv[SERVICE_CONTROL_OPTION], INSTALL.data())) 
		{
			return Service::get_instance().Install();
		}
		else if (!wcscmp(argv[SERVICE_CONTROL_OPTION], START.data()))
		{
			return Service::get_instance().Start();
		}
		else if (!wcscmp(argv[SERVICE_CONTROL_OPTION], STOP.data()))			
		{
			return Service::get_instance().Stop();
		}
		else if (!wcscmp(argv[SERVICE_CONTROL_OPTION], RESTART.data()))		
		{
			return Service::get_instance().Restart();
		}
		else if (!wcscmp(argv[SERVICE_CONTROL_OPTION], UNINSTALL.data()))		
		{
			return Service::get_instance().Uninstall();
		}
		else 
		{
			LOG_ERROR << "Invalid input";
		}
	}	
	return false;
}
