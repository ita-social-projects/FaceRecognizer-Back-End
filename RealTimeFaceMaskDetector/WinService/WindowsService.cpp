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
			LOG_ERROR << "OpenControlManager: OpenSCManager: ERROR " << GetLastError();
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
			LOG_ERROR << "GetServiceFromSCM: OpenService: ERROR " << GetLastError();
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
std::wstring Service::GetPathToModule()
{
	wchar_t path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	return std::wstring(path);
}

bool Service::CreateServiceInSCM(SC_HANDLE scm_handle)
{
	bool is_created;

	SERVER_EXE_PATH += L" " + SERVICE_NAME + L" " + GetPathToModule();

	service_handle = CreateService(
		scm_handle,										// SCM Handle
		Service::get_instance().s_service_name.c_str(), // Service Name
		Service::get_instance().s_service_name.c_str(),	// Display Name			
		SERVICE_ALL_ACCESS,								// Access Right
		SERVICE_WIN32_OWN_PROCESS,						// Service Type
		SERVICE_DEMAND_START,							// Service Start Type
		SERVICE_ERROR_NORMAL,							// Service Error Code
		SERVER_EXE_PATH.c_str(),						// Path to .exe
		nullptr,									    // Load ordering group
		nullptr,                                        // Tag ID
		nullptr,	                                    // Dependencies
		nullptr,										// Service start name (account)
		nullptr);                                       // Password

	if (!service_handle)
	{
		is_created = false;
		LOG_ERROR << "Install: CreateServiceInSCM: CreateService: ERROR " << GetLastError();
	}
	else
	{
		is_created = true;
		LOG_MSG << "Install: CreateServiceInSCM: succeeded :)";
		CloseHandleAndNull(service_handle);
	}
	CloseHandleAndNull(scm_handle);
	
	return is_created;
}

#pragma region Command Prompt
bool Service::Install()
{
	LOG_MSG << "Install: begin";

	if((scm_handle = OpenControlManager()) == nullptr)
	{
		return false;
	}
	bool is_created = CreateServiceInSCM(scm_handle);

	LOG_MSG << "Install: end";
	return is_created;
}

bool Service::Start()
{
	LOG_MSG << "Start: begin";

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

	LOG_MSG << "Start: end";
	return is_opened && is_started;
}

bool Service::Stop()
{
	LOG_MSG << "Stop: begin";

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

	LOG_MSG << "Stop: end";
	return is_opened && is_stopped;
}

bool Service::Restart()
{
	LOG_MSG << "Restart: begin";

	bool is_restarted = false;

	if (Service::get_instance().Stop() && Service::get_instance().Start())
	{
		is_restarted = true;
		LOG_MSG << "Restart: succeeded :)";
	}
	else
	{
		LOG_ERROR << "Restart: ERROR " << GetLastError();
	}
	LOG_MSG << "Restart: end";
	return is_restarted;
}

bool Service::Uninstall()
{
	LOG_MSG << "Uninstall: begin";
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

	LOG_MSG << "Uninstall: end";
	return is_opened && is_deleted;
}

#pragma endregion


void Service::TryStartService(SC_HANDLE service_handle, bool& is_started)
{
	
	auto path_to_current_module = GetPathToModule();
	const wchar_t* args[] = { s_service_name.c_str(), path_to_current_module.c_str()};
	if (!StartService(service_handle, 2, args))
	{
		LOG_ERROR << "TryStartService: StartService: ERROR " << GetLastError();
		is_started = false;
	}
	else
	{
		is_started = true;
		LOG_MSG << "TryStartService: succeeded :)";
	}
}

void Service::TryStopService(SC_HANDLE handle_open_service, SERVICE_STATUS_PROCESS status_process, bool& is_stopped)
{
	if (!ControlService(handle_open_service, SERVICE_CONTROL_STOP, reinterpret_cast<LPSERVICE_STATUS>(&status_process)))
	{
		int error_code = GetLastError();
		if (!error_code)
		{
			LOG_MSG << "Stop: succeeded :)";
		}
		else
		{
			LOG_ERROR << "Stop: ControlService: ERROR " << GetLastError();
			is_stopped = false;
		}
	}
}

void Service::TryDeleteService(SC_HANDLE handle_service, bool& is_deleted)
{
	if (!DeleteService(handle_service))
	{
		LOG_ERROR << "Uninstall: DeleteService: ERROR " << GetLastError();
		is_deleted = false;
	}
	else
	{
		is_deleted = true;
		LOG_MSG << "Uninstall: succeeded :)";
	}
}

