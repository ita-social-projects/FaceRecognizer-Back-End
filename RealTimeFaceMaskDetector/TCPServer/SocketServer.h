
#pragma once
#include <ws2tcpip.h>
#include "EncryptDecryptAES_ECBMode.h"
#include "Logger.h"
#include "SQLServer.h"
#define CONFIG_FILE "config.ini"
#pragma comment (lib, "Ws2_32.lib")

const char* const DEFAULT_PORT = "27015";
const int  DEFAULT_BUFLEN = 512;

class SocketServer
{
public:
	bool InitSocketServer();
	bool CreateListeningSocket();
	bool StartListening(bool& ret_value);
	bool ReceiveMessage(bool& ret_value);
	bool ShutdownServer();
	int GetMessageLength();

private:
	bool BindListeningSocket();
	bool AcceptConnection();
	bool SendMessage();

	/*return path to TCPServer.exe file*/
	std::filesystem::path GetCurrentPath();
	
	/*Creates directory for received images*/
	bool SpecifyPathForPhotos();

	/*Creates and opens .png file with specific filename*/
	bool OpenParticularFile(std::ofstream& stream);

	/*This function will take current date & time and 
	initialise <file_specificator> with converted date*/
	void CreateFileNameSpecificator();

	bool ReceiveFullMessage();

	WSADATA wsaData;
	int m_func_result;
	SOCKET m_listen_socket = INVALID_SOCKET;
	SOCKET m_client_socket = INVALID_SOCKET;
	std::vector<char> m_buffer;
	addrinfo* m_host_info = nullptr;
	addrinfo hints;

	/*Holds date & time to be shown in photo file name*/
	std::string file_specificator;
	/*When listening socket is created, this variable will be set to <true>.
	When Server will shut down, varible will be set to <false> */
	bool server_is_up;
};

