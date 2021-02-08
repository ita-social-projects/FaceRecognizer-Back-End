#pragma once
#include <ws2tcpip.h>
#include "EncryptDecryptAES_ECBMode.h"
#include "Logger.h"
#include "SQLServer.h"
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
	bool ReceiveMessage();
	bool ShutdownServer();
	int GetMessageLength();

private:
	bool SendMessage();

	WSADATA wsaData;
	int m_func_result;
	SOCKET m_listen_socket = INVALID_SOCKET;
	SOCKET m_client_socket = INVALID_SOCKET;
	std::vector<char> m_buffer;
	addrinfo* m_host_info = nullptr;
	addrinfo hints;
};

