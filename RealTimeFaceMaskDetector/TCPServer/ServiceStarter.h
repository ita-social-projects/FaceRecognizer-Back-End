#pragma once
#include "SocketServer.h"

class ServiceStarter
{
public:
	explicit ServiceStarter();
	explicit ServiceStarter(const std::wstring& service_name, bool& service_exists);
	
	bool SetServiceName(const std::wstring& service_name);
	const std::wstring& GetServiceName();

	/*Entry point for service.
	Service Main function that will be called
	after StartServiceCntrlDispatcher.*/
	static bool ServiceMain(const unsigned short argc, const wchar_t* const argv[]);

private:
	/*Control handler function, registered by 
	RegisterCtrlHandlerFunction in the ServiceMain function.
	It will be called when function ControlService is called*/
	static void CtrlHandler(const unsigned long request);
	
	/*Updates s_service_status variable with arguments.
	This service does no provide ability to pause it and continue,
	so it will not repotr statuses like:
	SERVICE_CONTINUE_PENDING;
	SERVICE_PAUSE_PENDING;
	SERVICE_PAUSED;*/
	static bool ReportStatus(
		const unsigned short current_state,
		const unsigned short win32_exit_code,
		const unsigned short wait_hint);

	/*Service worker function.
	Creates an stop event to stop service and starts
	new thread for sevice work function (Creates server).*/
	static bool StartServiceWork();

	/*Opens SCM manager ant tryes to get 
	handle on service with service_name. 
	If handle is valid, returns true*/
	static bool CheckServiceExists(const std::wstring& service_name);
private:
	static void TryCreateServer(bool& is_started);
	static bool CreateServer(SocketServer& _server = server, bool wait_till_finish = false);
	static void StartServerInNewThread(bool& is_listening_started, SocketServer& _server = server, bool wait_till_finish = false);
	static void ShutdownServer();

	/*Friend function that starts server work.
	Can be used in service_main functin
	to start server NOT under WinServce.
	Definded in ServiceStarter.h*/
	friend bool RunServerItself();
private:
	/*Variables to control service and it's status*/
	 static std::wstring s_service_name;
	 static SERVICE_STATUS s_service_status;
	 static SERVICE_STATUS_HANDLE s_service_status_handle;
	 static HANDLE s_service_stop_event;

	 static SocketServer server;
};


bool RunServerItself()
{
	SocketServer server;
	return ServiceStarter::CreateServer( server, true);
}