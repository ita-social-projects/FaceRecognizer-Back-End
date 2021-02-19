#include <iostream>
#include "SocketServer.h"

bool ManageServer(unsigned argc, wchar_t* option[]);
bool StartServerWork();
bool RestartServerWork();
bool StopServerWork();
void ListenInNewThread(bool& is_listening_started);

int wmain(unsigned argc,  wchar_t* argv[])
{
	LOG_MSG << "Sevrer MAIN function BEGIN!!!";
	ManageServer(argc, argv);
	return 0;
}


bool ManageServer(unsigned argc, wchar_t* option[])
{
	if (argc > 1)
	{
		LOG_MSG << "Managing started";
		if (!wcscmp(option[SERVER_MANAGE_OPTION], START.data()))			return StartServerWork();
		else if (!wcscmp(option[SERVER_MANAGE_OPTION], STOP.data()))			return StopServerWork();
		else if (!wcscmp(option[SERVER_MANAGE_OPTION], RESTART.data()))		return RestartServerWork();
		else std::cerr << "Invalid input\n";
		LOG_MSG << "Managing ERRROOROROR";
	}
	return false;
}

bool StartServerWork()
{
	LOG_MSG << "CreateServer begin";

	bool is_server_initialized = SocketServer::getInstance().InitSocketServer();
	bool is_socked_created = SocketServer::getInstance().CreateListeningSocket();
	bool is_listening_started;
	SocketServer::getInstance().StartListening(is_listening_started);
	
	LOG_MSG << "CreateServer end";
	return is_server_initialized &&
		is_socked_created &&
		is_listening_started;
}

void ListenInNewThread(bool& is_listening_started)
{
	auto thread = std::thread([&]() {SocketServer::getInstance().StartListening(is_listening_started); });
	if(thread.joinable())
	{
		thread.detach();
	}
}

bool RestartServerWork()
{
	LOG_MSG << "Restarting Server";
	return StopServerWork() && StartServerWork();
}

bool StopServerWork()
{
	LOG_MSG << "Stopping server";
	return SocketServer::getInstance().ShutdownServer();
}