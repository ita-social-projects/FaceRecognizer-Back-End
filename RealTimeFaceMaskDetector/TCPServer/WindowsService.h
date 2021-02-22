#pragma once
#include "SocketServer.h"

constexpr unsigned SERVICE_START_WAIT_HINT = 3030;
constexpr unsigned DEFAULT_WAIT_HINT = 0;
constexpr unsigned DEFAULT_SPECIFIC_EXIT_CODE_VALUE = 0;
constexpr unsigned BEGIN_CONTROL_ACCEPTED_VALUE = 0;
constexpr unsigned BEGIN_CHECK_POINT_VALUE = 0;


class Service
{
public:
	Service(const Service&) = delete;
	Service(Service&&) = delete;
	Service& operator=(const Service&) = delete;
	Service& operator=(Service&&) = delete;

	static const std::wstring& get_service_name();
	static Service& get_instance();

	static void set_service_name(const std::wstring&);

	static bool Main(const unsigned short argc, const wchar_t* const argv[]);
	static bool Install();
	static bool Start();
	static bool Stop();
	static bool Restart();
	static bool Uninstall();

private:
	Service() = default;

	static bool CtrlHandler(const unsigned short control);
	static bool Initialize(const unsigned short argc, const wchar_t* const argv[]);
	static bool ReportStatus(
		const unsigned short current_state,
		const unsigned short win32_exit_code,
		const unsigned short wait_hint);
	static void StartServerWork(bool&);
	static bool CreateServer();
	static void TryCreateServer(bool& is_started);
	static void TryStopService(SC_HANDLE handle_open_service, SERVICE_STATUS_PROCESS status_process, bool& is_stopped);
	static void TryDeleteService(SC_HANDLE handle_service, bool& is_deleted);
	static void TryStartService(SC_HANDLE handle_open_service ,bool& is_started);

	static bool ShutdownServer();

	static std::unique_ptr<SocketServer> s_socket_server;
	static SERVICE_STATUS s_service_status;
	static SERVICE_STATUS_HANDLE s_service_status_handle;
	static HANDLE s_service_stop_event;
	static std::shared_ptr<Service> s_instance;
	static std::wstring s_service_name;
};