#pragma once
#include <ws2tcpip.h>
#include "Logger.h"
#include "SocketServer.h"
#include "SocketServer.cpp"
#include <string_view>
#include <fstream>
constexpr unsigned SERVICE_START_WAIT_HINT = 3030;
constexpr unsigned DEFAULT_WAIT_HINT = 0;
constexpr unsigned DEFAULT_SPECIFIC_EXIT_CODE_VALUE = 0;
constexpr unsigned BEGIN_CONTROL_ACCEPTED_VALUE = 0;
constexpr unsigned BEGIN_CHECK_POINT_VALUE = 0;

std::ofstream out;



const wchar_t SERVER_EXE_PATH[] = L"D:\\Programing\\Projects\\Real-Time-Face-Mask-Detector-Server\\"
									"RealTimeFaceMaskDetector\\x64\\Debug\\TCPServer.exe";


/*Singletone class that provides comunication with SCM 
and creates service to run as server*/
class Service
{
public:
	/*Delete constructors ('cause Singletone)*/
	Service(const Service&) = delete;
	Service(Service&&) = delete;
	Service& operator=(const Service&) = delete;
	Service& operator=(Service&&) = delete;

	static const std::wstring& get_service_name();
	static Service& get_instance();

	static void set_service_name(const std::wstring&);

	/*Entry point for service.
	Service Main function that will be called 
	after StartServiceCntrlDispatcher.*/
	static bool Main(const unsigned short argc, const wchar_t* const argv[]);

	/*Functions to interract with 
	service from command prompt*/
	static bool Install();
	static bool Start();
	static bool Stop();
	static bool Restart();
	static bool Uninstall();
private:
	Service() = default;

	static void CtrlHandler(const unsigned short control);

	/*Service worker function.
	Creates an stop event to stop service and starts
	new thread for sevice work function (Creates server).*/
	static bool Initialize(const unsigned short argc, const wchar_t* const argv[]);
	
	/*
	Updates s_service_status variable with arguments. 
	This service does no provide ability to pause it and continue,
	so it will not repotr statuses like: 
	SERVICE_CONTINUE_PENDING;
	SERVICE_PAUSE_PENDING;
	SERVICE_PAUSED;*/
	static bool ReportStatus(
		const unsigned short current_state,
		const unsigned short win32_exit_code,
		const unsigned short wait_hint);

	/*Calls StartService function and 
	if suceed takes up the server*/
	static void TryStartService(SC_HANDLE handle_open_service ,bool& is_started);

	/*Stops service, using ControlService function.*/
	static void TryStopService(SC_HANDLE handle_open_service, SERVICE_STATUS_PROCESS status_process, bool& is_stopped);
	
	/*Deletes service. Required to stop 
	service by (TryStopService) firstly*/
	static void TryDeleteService(SC_HANDLE handle_service, bool& is_deleted);
	
	/*function to comunicate with server*/
	static void StartServerWork(bool&);
	static bool CreateServer();
	static void TryCreateServer(bool& is_started);
	static bool ShutdownServer();

	/*Variables to control service and it's status*/
	static std::shared_ptr<Service> s_instance;
	static std::wstring s_service_name;
	static SERVICE_STATUS s_service_status;
	static SERVICE_STATUS_HANDLE s_service_status_handle;
	static HANDLE s_service_stop_event;


};