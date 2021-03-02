#pragma once
#include <ws2tcpip.h>
#include "Logger.h"
#include <string_view>

const std::wstring_view
INSTALL = L"install",
START = L"start",
STOP = L"stop",
RESTART = L"restart",
UNINSTALL = L"uninstall";

const wchar_t SERVICE_NAME[] = L"AA_server";
const wchar_t SERVER_EXE_PATH[] = L"E:\\Tolik\\c++\\Real-Time-Face-Mask-Detector\\RealTimeFaceMaskDetector\\x64\\Debug\\TCPServer.exe AA_server";

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

	/*Functions to interract with 
	service from command prompt*/
	bool Install();
	bool Start();
	bool Stop();
	bool Restart();
	bool Uninstall();
private:
	Service() = default;

	/*heturns handle to SCM database*/
	SC_HANDLE OpenControlManager();
	
	/*returns handle to service if it exists*/
	SC_HANDLE GetServiceFromSCM(SC_HANDLE scm_handle);

	void CloseHandleAndNull(SC_HANDLE handle);

	bool CreateServiceInSCM(SC_HANDLE scm_handle);
	/*Calls StartService function and 
	if suceed takes up the server*/
	void TryStartService(SC_HANDLE handle_open_service ,bool& is_started);

	/*Stops service, using ControlService function.*/
	void TryStopService(SC_HANDLE handle_open_service, SERVICE_STATUS_PROCESS status_process, bool& is_stopped);
	
	/*Deletes service. Required to stop 
	service by (TryStopService) firstly*/
	void TryDeleteService(SC_HANDLE handle_service, bool& is_deleted);

	/*Variables to control service and it's status*/
	static std::shared_ptr<Service> s_instance;
	static std::wstring s_service_name;
	static SC_HANDLE scm_handle;
	static SC_HANDLE service_handle;
};