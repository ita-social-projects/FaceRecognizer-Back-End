#include "WindowsService.h"

#pragma region Variables
std::shared_ptr<Service>Service::s_instance{};
std::wstring Service::s_service_name{};
SC_HANDLE Service::scm_handle = nullptr;
SC_HANDLE Service::service_handle = nullptr;
#pragma endregion


#pragma region Getters and Seters

const std::wstring& Service::get_service_name()
{
	return s_service_name;
}

Service& Service::get_instance()
{
	if (!s_instance.get())
	{
		s_instance = std::shared_ptr<Service>{ new Service };
	}
	return *s_instance;
}

void Service::set_service_name(const std::wstring& str)
{
	s_service_name = str;
}

#pragma endregion

SC_HANDLE Service::OpenControlManager()
{
	if (scm_handle == nullptr)
	{
		/*Establishes a connection to the service control manager on the specified computer
		and opens the specified service control manager database.*/
		scm_handle = OpenSCManager(
			nullptr,					// Local Machine
			nullptr,					// By default Database 
			SC_MANAGER_ALL_ACCESS);		// Access Right

		if (!scm_handle)
		{
			os << "OpenControlManager: OpenSCManager: ERROR " << GetLastError();
			return nullptr;
		}
	}
	return scm_handle;
}

SC_HANDLE Service::GetServiceFromSCM(SC_HANDLE scm_handle)
{
	if(service_handle == nullptr)
	{
		service_handle = OpenService(
			scm_handle,											// SCM Handle
			Service::get_instance().s_service_name.c_str(),		// Service Name
			SC_MANAGER_ALL_ACCESS);								// Access Right

		if (!service_handle)
		{
			os << "GetServiceFromSCM: OpenService: ERROR " << GetLastError();
			return nullptr;
		}
	}
	return service_handle;
}

void Service::CloseHandleAndNull(SC_HANDLE handle)
{
	CloseServiceHandle(handle);
	handle = nullptr;
}
bool Service::CreateServiceInSCM(SC_HANDLE scm_handle)
{
	bool is_created;

	service_handle = CreateService(
		scm_handle,										// SCM Handle
		Service::get_instance().s_service_name.c_str(), // Service Name
		Service::get_instance().s_service_name.c_str(),	// Display Name			
		SERVICE_ALL_ACCESS,								// Access Right
		SERVICE_WIN32_OWN_PROCESS,						// Service Type
		SERVICE_DEMAND_START,							// Service Start Type
		SERVICE_ERROR_NORMAL,							// Service Error Code
		SERVER_EXE_PATH,								// Path to .exe
		nullptr,									    // Load ordering group
		nullptr,                                        // Tag ID
		nullptr,	                                    // Dependencies
		nullptr,						                // Service start name (account)
		nullptr);                                       // Password

	if (!service_handle)
	{
		is_created = false;
		os << "Install:CreateServiceInSCM: CreateService: ERROR " << GetLastError();
	}
	else
	{
		is_created = true;
		os << "Install:CreateServiceInSCM: succeeded :)";
		CloseHandleAndNull(service_handle);
	}
	CloseHandleAndNull(scm_handle);
	
	return is_created;
}

#pragma region Command Prompt
bool Service::Install()
{
	os << "Install begin";

	if((scm_handle = OpenControlManager()) == nullptr)
	{
		return false;
	}
	bool is_created = CreateServiceInSCM(scm_handle);

	os << "\nInstall end\n";
	os.close();
	return is_created;
}

bool Service::Start()
{
	os << "Start begin\n";

	bool is_opened = true, is_started = true;

	if((scm_handle = OpenControlManager()) == nullptr)
	{
		return false;
	}
	if((service_handle = GetServiceFromSCM(scm_handle)) == nullptr)
	{
		is_opened = false;
	}
	else
	{
		TryStartService(service_handle, is_started);
		CloseHandleAndNull(service_handle);
	}
	CloseHandleAndNull(scm_handle);

	os << "Start end\n";
	os.close();
	return is_opened && is_started;
}

bool Service::Stop()
{
	os << "Stop begin";

	SERVICE_STATUS_PROCESS status_process{};
	bool is_opened = true, is_stopped = true;

	if ((scm_handle = OpenControlManager()) == nullptr)
	{
		return false;
	}

	if ((service_handle = GetServiceFromSCM(scm_handle)) == nullptr)
	{
		is_opened = false;
	}
	else
	{
		TryStopService(service_handle, status_process, is_stopped);
		CloseHandleAndNull(service_handle);
	}
	CloseHandleAndNull(scm_handle);

	os << "Stop end";
	os.close();
	return is_opened && is_stopped;
}

bool Service::Restart()
{
	os << "Restart begin";

	bool is_restarted = false;

	if (Service::get_instance().Stop() && Service::get_instance().Start())
	{
		is_restarted = true;
		os << "Restart: succeeded :)";
	}
	else
	{
		os << "Restart: ERROR " << GetLastError();
	}
	os << "Restart end";
	return is_restarted;
}

bool Service::Uninstall()
{
	os << "Uninstall begin";
	bool is_opened = true, is_deleted = true;

	if ((scm_handle = OpenControlManager()) == nullptr)
	{
		return false;
	}

	if ((service_handle = GetServiceFromSCM(scm_handle)) == nullptr)
	{
		is_opened = false;
	}
	else
	{
		TryDeleteService(service_handle, is_deleted);
		CloseHandleAndNull(service_handle);
	}
	CloseHandleAndNull(scm_handle);

	os << "Uninstall end";
	return is_opened && is_deleted;
}

#pragma endregion


void Service::TryStartService(SC_HANDLE service_handle, bool& is_started)
{
	const wchar_t* args[] = { s_service_name.c_str() };
	if (!StartService(service_handle, 1, args))
	{
		os << "Start: StartService: ERROR " << GetLastError() << std::endl;
		os.close();
		is_started = false;
	}
	else
	{
		is_started = true;
		os << "Start succeeded :)";
	}
}

void Service::TryStopService(SC_HANDLE handle_open_service, SERVICE_STATUS_PROCESS status_process, bool& is_stopped)
{
	if (!ControlService(handle_open_service, SERVICE_CONTROL_STOP, reinterpret_cast<LPSERVICE_STATUS>(&status_process)))
	{
		int error_code = GetLastError();
		if (!error_code)
		{
			os << "Stop: succeeded :)";
		}
		else
		{
			os << "Stop: ControlService: ERROR " << GetLastError();
			is_stopped = false;
		}
	}
}

void Service::TryDeleteService(SC_HANDLE handle_service, bool& is_deleted)
{
	if (!DeleteService(handle_service))
	{
		os << "Uninstall: DeleteService: ERROR " << GetLastError();
		is_deleted = false;
	}
	else
	{
		is_deleted = true;
		os << "Uninstall: succeeded :)";
	}
}

