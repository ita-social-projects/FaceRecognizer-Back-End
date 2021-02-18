#include "WindowsService.h"
#include <thread>
#pragma region Variables

SocketServer Service::s_socket_server{};
SERVICE_STATUS Service::s_service_status{};
SERVICE_STATUS_HANDLE Service::s_service_status_handle{};
HANDLE Service::s_service_stop_event{};
std::shared_ptr<Service>Service::s_instance{};
std::wstring Service::s_service_name{};

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

#pragma region Service Control Manager

// The entry point for a service
bool Service::Main(const unsigned short argc, const wchar_t* const argv[])
{
	LOG_MSG << "Main begin";

	s_service_status_handle = RegisterServiceCtrlHandler(Service::get_instance().s_service_name.c_str(),
		reinterpret_cast<LPHANDLER_FUNCTION>(CtrlHandler));

	if (!s_service_status_handle)
	{
		LOG_ERROR << "Main: RegisterServiceCtrlHandler ERROR " << GetLastError();
		return false;
	}

	ZeroMemory(&s_service_status, sizeof(s_service_status));
	s_service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	s_service_status.dwServiceSpecificExitCode = DEFAULT_SPECIFIC_EXIT_CODE_VALUE;

	ReportStatus(SERVICE_START_PENDING, NO_ERROR, SERVICE_START_WAIT_HINT); // Tell the controller we are starting
	return Initialize(argc, argv);
}

bool Service::ReportStatus(const unsigned short current_state, const unsigned short win32_exit_code, const unsigned short wait_hint)
{
	LOG_MSG << "ReportStatus begin";

	static unsigned short s_check_point = 1;

	s_service_status.dwCurrentState = current_state;
	s_service_status.dwWin32ExitCode = win32_exit_code;
	s_service_status.dwWaitHint = wait_hint;

	if (current_state == SERVICE_START_PENDING) // Service is about to start
	{
		s_service_status.dwControlsAccepted = BEGIN_CONTROL_ACCEPTED_VALUE;
	}
	else
	{
		s_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	}

	if (current_state == SERVICE_RUNNING || current_state == SERVICE_STOPPED)  // Progress for Service operation
	{
		s_service_status.dwCheckPoint = BEGIN_CHECK_POINT_VALUE;
	}
	else
	{
		s_service_status.dwCheckPoint = s_check_point++;
	}	

	if (!SetServiceStatus(s_service_status_handle, &s_service_status))
	{
		LOG_ERROR << "Report Status: ERROR " << GetLastError();
		return false;
	}

	LOG_MSG << "ReportStatus end";
	return true;
}


bool Service::Initialize(const unsigned short argc, const wchar_t* const argv[])
{
	LOG_MSG << "Initialize begin";

	s_service_stop_event = CreateEvent( // Create a service stop event to wait on later
		nullptr,						// Security Attributes
		true,							// MANUAL Reset Event
		false,							// Non-Signaled
		nullptr);						// Name of Event

	if (!s_service_stop_event)
	{
		return ReportStatus(SERVICE_STOPPED, NO_ERROR, DEFAULT_WAIT_HINT);
	}

	ReportStatus(SERVICE_RUNNING, NO_ERROR, DEFAULT_WAIT_HINT);	// Start the thread that will perform the main task of the service

	CreateServer();
	std::cout << " After CreateServer..." << std::endl;
	WaitForSingleObject(s_service_stop_event, INFINITE); // Wait event to be Signaled	

	LOG_MSG << "Initialize end";
	LOG_MSG << "Main end";
	return ReportStatus(SERVICE_STOPPED, NO_ERROR, DEFAULT_WAIT_HINT);
}

void Service::StartServerWork(bool& is_listening_started)
{
	std::thread listen_multiple_clients([&]() 
		{
			s_socket_server.StartListening(is_listening_started);
			if (!is_listening_started)
			{
				
				return;
			}
		});
	if (listen_multiple_clients.joinable())
	{
		listen_multiple_clients.join();
	}
}

bool Service::CreateServer()
{
	LOG_MSG << "CreateServer begin";
	bool is_server_initialized = false;
	is_server_initialized = s_socket_server.InitSocketServer();
	bool is_socked_created =false;
	is_socked_created = s_socket_server.CreateListeningSocket();
	bool is_listening_started=false;
	
	StartServerWork(is_listening_started);
	LOG_MSG << "CreateServer end";
	s_socket_server.ShutdownServer();
	return is_server_initialized && 
		is_socked_created &&
		is_listening_started;
}

void Service::TryCreateServer(bool& is_started)
{
	is_started = CreateServer();
	if (!is_started)
	{
		LOG_WARNING << "Start: CreateServer: ERROR " << GetLastError();
	}
	else
	{
		LOG_MSG << "Start: succeeded :)";
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
		LOG_MSG << "Uninstall: succeeded :)";
	}
}

void Service::TryStartService(SC_HANDLE handle_open_service, bool& is_started)
{
	if (!StartService(handle_open_service, false, nullptr))
	{
		LOG_ERROR << "Start: StartService: ERROR " << GetLastError();
		is_started = false;
	}
	else
	{
		TryCreateServer(is_started);
	}
}

bool Service::ShutdownServer()
{
	return s_socket_server.ShutdownServer();	
}

bool Service::CtrlHandler(const unsigned short request)
{
	LOG_MSG << "CtrlHandler begin";

	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		if (s_service_status.dwCurrentState != SERVICE_RUNNING) break;

		ShutdownServer();
		return ReportStatus(SERVICE_STOPPED, NO_ERROR, DEFAULT_WAIT_HINT);

	default: break;
	}
	LOG_MSG << "CtrlHandler end";
	return false;
}

#pragma endregion

#pragma region Command Prompt

bool Service::Install()
{
	LOG_MSG << "Install begin";

	SC_HANDLE handle_SCM = nullptr;
	SC_HANDLE handle_service = nullptr;
	wchar_t path[MAX_PATH];

	/*Get the path to Executable file from SCM.
	If param 0 is nullptr, writes path to current .exe */
	if (!GetModuleFileName(nullptr, path, MAX_PATH))
	{
		LOG_ERROR << "Install: GetModuleFileName: ERROR " << GetLastError();
		return false;
	};

	/*Establishes a connection to the service control manager on the specified computer
	and opens the specified service control manager database.*/
	handle_SCM = OpenSCManager(
		nullptr,					// Local Machine
		nullptr,					// By default Database 
		SC_MANAGER_ALL_ACCESS);		// Access Right

	if (!handle_SCM)
	{
		LOG_ERROR << "Install: OpenSCManager: ERROR " << GetLastError();
		return false;
	}

	handle_service = CreateService(
		handle_SCM,										// SCM Handle
		Service::get_instance().s_service_name.c_str(), // Service Name
		Service::get_instance().s_service_name.c_str(),	// Display Name			
		SERVICE_ALL_ACCESS,								// Access Right
		SERVICE_WIN32_OWN_PROCESS,						// Service Type
		SERVICE_DEMAND_START,							// Service Start Type
		SERVICE_ERROR_NORMAL,							// Service Error Code
		path,											// Path to .exe
		nullptr,									    // Load ordering group
		nullptr,                                        // Tag ID
		nullptr,	                                    // Dependencies
		nullptr,						                // Service start name (account)
		nullptr);                                       // Password

	bool is_created = true;

	if (!handle_service)
	{
		LOG_ERROR << "Install: CreateService: ERROR " << GetLastError();
		is_created = false;
	}
	else
	{
		LOG_MSG << "Install: succeeded :)";
		CloseServiceHandle(handle_service);
	}
	CloseServiceHandle(handle_SCM);

	LOG_MSG << "Install end";
	return is_created;
}

bool Service::Start()
{
	LOG_MSG << "Start begin";

	SC_HANDLE handle_open_SCM = nullptr;
	SC_HANDLE handle_open_service = nullptr;

	handle_open_SCM = OpenSCManager(
		nullptr,					// Local Machine
		nullptr,					// By default Database (SERVICES_ACTIVE_DATABASE)
		SC_MANAGER_ALL_ACCESS);		// Access Right

	if (!handle_open_SCM)
	{
		LOG_ERROR << "Start: OpenSCManager: ERROR " << GetLastError();
		return false;
	}

	bool is_opened = true, is_started = true, is_stopped = false;

	handle_open_service = OpenService(
		handle_open_SCM,									// SCM Handle
		Service::get_instance().s_service_name.c_str(),		// Service Name
		SC_MANAGER_ALL_ACCESS);								// Access Right

	if (!handle_open_service)
	{
		LOG_ERROR << "Start: OpenService: ERROR " << GetLastError();
		is_opened = false;
	}
	else if (is_opened)
	{
		TryStartService(handle_open_service, is_started);

		if (!is_started)
		{
			std::cout << "Shuting down Server..." << std::endl;
			if (ShutdownServer())
			{
				std::cout << "Shut down Server..." << std::endl;
			}
			Stop();
			std::cout << "Stop service..." << std::endl;
			is_started = true;
		}
	}
	try
	{
		CloseServiceHandle(handle_open_service);
		std::cout << "Close Service Handle ..." << std::endl;
	}
	catch (const std::exception&)
	{
		std::cout << GetLastError() << std::endl;
	}
	CloseServiceHandle(handle_open_SCM);
	std::cout << "Close Service Handle handle_open_SCM ..." << std::endl;
	LOG_MSG << "Start end";
	return is_opened && is_started;
}

bool Service::Stop()
{
	LOG_MSG << "Stop begin";

	ShutdownServer();

	SERVICE_STATUS_PROCESS status_process{};
	SC_HANDLE open_SCM = nullptr;
	SC_HANDLE open_service = nullptr;

	open_SCM = OpenSCManager(
		nullptr,					// Local Machine
		nullptr,					// By default Database (SERVICES_ACTIVE_DATABASE)
		SC_MANAGER_ALL_ACCESS);		// Access Right

	if (!open_SCM)
	{
		LOG_ERROR << "Stop: OpenSCManager: ERROR " << GetLastError();
		return false;
	}

	bool is_opened = true, is_stopped = true;

	open_service = OpenService(
		open_SCM,											// SCM Handle
		Service::get_instance().s_service_name.c_str(),		// Service Name
		SC_MANAGER_ALL_ACCESS);								// Access Right

	if (!open_service)
	{
		LOG_ERROR << "Stop: OpenService: ERROR " << GetLastError();
		is_opened = false;
	}
	else if (is_opened)
	{
		TryStopService(open_service, status_process, is_stopped);
		CloseServiceHandle(open_service);
	}
	CloseServiceHandle(open_SCM);

	LOG_MSG << "Stop end";
	return is_opened && is_stopped;
}

bool Service::Restart()
{
	LOG_MSG << "Restart begin";

	bool is_restarted = false;

	if (Service::get_instance().Stop() && Service::get_instance().Start())
	{
		LOG_MSG << "Restart: succeeded :)";
		is_restarted = true;
	}
	else
	{
		LOG_ERROR << "Restart: ERROR " << GetLastError();
	}

	LOG_MSG << "Restart end";
	return is_restarted;
}

bool Service::Uninstall()
{
	LOG_MSG << "Uninstall begin";

	SC_HANDLE handle_SCM = nullptr;
	SC_HANDLE handle_service = nullptr;

	handle_SCM = OpenSCManager(
		nullptr,					// Local Machine
		nullptr,					// By default Database (SERVICES_ACTIVE_DATABASE)
		SC_MANAGER_ALL_ACCESS);		// Access Right

	if (!handle_SCM)
	{
		LOG_ERROR << "Uninstall: OpenSCManager: ERROR " << GetLastError();
		return false;
	}

	bool is_opened = true, is_deleted = true;

	handle_service = OpenService(
		handle_SCM,										// SCM Handle
		Service::get_instance().s_service_name.c_str(), // Service Name
		SERVICE_ALL_ACCESS);							// Access Right

	if (!handle_service)
	{
		LOG_ERROR << "Uninstall: OpenSCManager: ERROR " << GetLastError();
		is_opened = false;
	}
	else if (is_opened)
	{
		TryDeleteService(handle_service, is_deleted);
		CloseServiceHandle(handle_service);
	}
	CloseServiceHandle(handle_SCM);

	LOG_MSG << "Uninstall end";
	return is_opened && is_deleted;
}

#pragma endregion