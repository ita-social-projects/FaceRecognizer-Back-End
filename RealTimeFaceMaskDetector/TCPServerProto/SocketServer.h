#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

const char* const DEFAULT_PORT = "27015";
const int  DEFAULT_BUFLEN = 512;

class SocketServer
{
public:
	bool InitSocketServer();
	bool CreateListeningSocket();
	bool StartListening();
	bool AcceptConnection();
	int GetMessageLength();
	bool ReceiveMessage();
	bool ShutdownServer();

private:
	WSADATA wsaData;
	int m_func_result;

	SOCKET m_listen_socket = INVALID_SOCKET;
	SOCKET m_client_socket = INVALID_SOCKET;

	std::vector<char> m_buffer;

	addrinfo* m_host_info = nullptr;
	addrinfo hints;
};


