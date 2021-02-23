#include "WindowsService.h"

constexpr short SERVICE_CONTROL_OPTION = 1;

bool InteractWithServiceUsingCommandPrompt(const unsigned argc, const wchar_t* const argv[]);

int wmain(unsigned argc, wchar_t* argv[])
{
	os.open("D:\\Programing\\Projects\\Real-Time-Face-Mask-Detector-Server\\RealTimeFaceMaskDetector\\x64\\Debug\\AAAsevice.txt",
		std::ios_base::app);
	
	os << "wmain\n";

	Service::set_service_name(SERVICE_NAME);

	InteractWithServiceUsingCommandPrompt(argc, argv);
	os.close();
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
			os << "Invalid input\n";
		}
	}	
	return false;
}
