#include "ServiceStarter.h"
#include <memory>
constexpr unsigned SERVICE_START_WAIT_HINT = 3030;
constexpr unsigned DEFAULT_WAIT_HINT = 0;
constexpr unsigned DEFAULT_SPECIFIC_EXIT_CODE_VALUE = 0;
constexpr unsigned BEGIN_CONTROL_ACCEPTED_VALUE = 0;
constexpr unsigned BEGIN_CHECK_POINT_VALUE = 0;

std::wstring ServiceStarter::s_service_name = {};
SERVICE_STATUS ServiceStarter::s_service_status = {};
SERVICE_STATUS_HANDLE ServiceStarter::s_service_status_handle = {};
HANDLE ServiceStarter::s_service_stop_event = {};
SocketServer ServiceStarter::server = {};

ServiceStarter::ServiceStarter()
{
	s_service_name = L"";
	s_service_status = {};
	s_service_status_handle = {};
	s_service_stop_event = {};
}

ServiceStarter::ServiceStarter(const std::wstring& service_name, bool& service_exists)
{
	if(!CheckServiceExists(service_name))
	{
		service_exists = false;
	}
	LOG_MSG << "ServiceStarter: service name found";
	service_exists = true;
	s_service_name = service_name;
}

bool ServiceStarter::SetServiceName(const std::wstring& service_name)
{
	if (!CheckServiceExists(service_name))
	{
		return false;
	}
	LOG_MSG << "SetServiceName: service name found";
	s_service_name = service_name;
	return true;
}

const std::wstring& ServiceStarter::GetServiceName()
{
	return s_service_name;
}

bool ServiceStarter::ServiceMain(const unsigned short argc, const wchar_t* const argv[])
{
	LOG_MSG << "ServiceMain: begin";

	s_service_status_handle = RegisterServiceCtrlHandler(
		s_service_name.c_str(), 
		reinterpret_cast<LPHANDLER_FUNCTION>(CtrlHandler));

	if (!s_service_status_handle)
	{
		LOG_ERROR << "ServiceMain: RegisterServiceCtrlHandler ERROR " << GetLastError();
		return false;
	}

	ZeroMemory(&s_service_status, sizeof(s_service_status));
	s_service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	s_service_status.dwServiceSpecificExitCode = DEFAULT_SPECIFIC_EXIT_CODE_VALUE;

	ReportStatus(SERVICE_START_PENDING, NO_ERROR, SERVICE_START_WAIT_HINT); // Tell the scm we are starting
	return StartServiceWork();
}

void ServiceStarter::CtrlHandler(const unsigned long request)
{
	LOG_MSG << "CtrlHandler: begin";

	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		if (s_service_status.dwCurrentState != SERVICE_RUNNING)
		{
			break;
		}
		ShutdownServer();

		ReportStatus(SERVICE_STOPPED, NO_ERROR, DEFAULT_WAIT_HINT);
		break;
	default:
		break;
	}

	LOG_MSG << "CtrlHandler: end";
}

bool ServiceStarter::ReportStatus(const unsigned short current_state, const unsigned short win32_exit_code, const unsigned short wait_hint)
{
	LOG_MSG << "ReportStatus: begin";

	static unsigned short s_check_point = 1;

	s_service_status.dwCurrentState = current_state;
	s_service_status.dwWin32ExitCode = win32_exit_code;
	s_service_status.dwWaitHint = wait_hint;

	if (current_state == SERVICE_START_PENDING) // Service is about to start
	{
		LOG_MSG << "ReportStatus: SERVICE_START_PENDING";
		s_service_status.dwControlsAccepted = BEGIN_CONTROL_ACCEPTED_VALUE;
	}
	else
	{
		s_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	}

	if (current_state == SERVICE_RUNNING || current_state == SERVICE_STOPPED)  // Progress for Service operation
	{
		LOG_MSG << "ReportStatus: SERVICE_RUNNING || SERVICE_STOPPED";
		s_service_status.dwCheckPoint = BEGIN_CHECK_POINT_VALUE;
	}
	else
	{
		s_service_status.dwCheckPoint = s_check_point++;
	}

	if (!SetServiceStatus(s_service_status_handle, &s_service_status))
	{
		LOG_ERROR << "Report Status: SetServiceStatus: ERROR " << GetLastError();
		return false;
	}

	LOG_MSG << "ReportStatus: end";
	return true;
}

bool ServiceStarter::StartServiceWork()
{
	LOG_MSG << "StartServiceWork: begin";

	s_service_stop_event = CreateEvent( // Create a service stop event to wait on later
		nullptr,						// Security Attributes
		true,							// MANUAL Reset Event
		false,							// Non-Signaled
		nullptr);						// Name of Event

	if (!s_service_stop_event)
	{
		LOG_ERROR << "StartServiceWork: CreateEvent: ERROR " << GetLastError();
		return ReportStatus(SERVICE_STOPPED, NO_ERROR, DEFAULT_WAIT_HINT);
	}

	ReportStatus(SERVICE_RUNNING, NO_ERROR, DEFAULT_WAIT_HINT);
	
	// Start the thread that will perform the main task of the service
	bool is_started = false;
	TryCreateServer(is_started);

/*	HANDLE newHandle;
	RegisterWaitForSingleObject(
		&newHandle, 
		s_service_stop_event, 
		reinterpret_cast<WAITORTIMERCALLBACK>(nullptr,false), 
		nullptr, 
		INFINITE, 
		WT_EXECUTEONLYONCE);

	UnregisterWait(newHandle); // Wait event to be Signaled	in other thread
*/
	WaitForSingleObject(s_service_stop_event, INFINITE);
	LOG_MSG << "StartServiceWork: end";
	LOG_MSG << "ServiceMain: end";
	return ReportStatus(SERVICE_STOPPED, NO_ERROR, DEFAULT_WAIT_HINT);
}

/*Functon just for creation of instance 
of this class with existing service*/
bool ServiceStarter::CheckServiceExists(const std::wstring& service_name)
{
	bool service_exists = true;

	auto scm_handle = OpenSCManager(
		nullptr,					// Local Machine
		nullptr,					// By default Database 
		SC_MANAGER_ALL_ACCESS);
	if (!scm_handle)
	{
		LOG_ERROR << "CheckServiceExists : OpenSCManager : failed to open manager : " << GetLastError();
		return false;
	}

	auto service_handle = OpenService(
		scm_handle,					// SCM Handle
		service_name.c_str(),		// Service Name
		SC_MANAGER_ALL_ACCESS);
	if (!service_handle)
	{
		LOG_ERROR << "CheckServiceExists : OpenService : failed to open service : " << GetLastError();
		service_exists = false;
		CloseServiceHandle(service_handle);
	}
	CloseServiceHandle(scm_handle);
	return service_exists;
}

//------Functions to start server--------
void ServiceStarter::TryCreateServer(bool& is_started)
{
	LOG_MSG << "TryCreateServer: begin";
	is_started = CreateServer();
	if (!is_started)
	{
		LOG_WARNING << "TryCreateServer: ERROR " << GetLastError();
	}
	else
	{
		LOG_MSG << "TryCreateServer: succeeded :)";
	}
	LOG_MSG << "TryCreateServer: end";
}

bool ServiceStarter::CreateServer(SocketServer& _server, bool wait_till_finish)
{
	LOG_MSG << "CreateServer: begin";
	bool is_server_initialized = _server.InitSocketServer();
	bool is_socked_created = _server.CreateListeningSocket();
	bool is_listening_started;

	StartServerInNewThread(is_listening_started, _server, wait_till_finish);

	LOG_MSG << "CreateServer: end";
	return is_server_initialized &&
		is_socked_created &&
		is_listening_started;
}

void ServiceStarter::StartServerInNewThread(bool& is_listening_started, SocketServer& _server, bool wait_till_finish)
{
	std::thread listen_multiple_clients([&]() {_server.StartListening(is_listening_started); });
	if (listen_multiple_clients.joinable())
	{
		wait_till_finish
		?
		listen_multiple_clients.join()
		:
		listen_multiple_clients.detach();
	}
}

void ServiceStarter::ShutdownServer()
{
	server.ShutdownServer();
}
