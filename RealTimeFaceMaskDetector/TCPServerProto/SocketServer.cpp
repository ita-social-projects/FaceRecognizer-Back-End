#pragma once
#include "SocketServer.h"


bool SocketServer::InitSocketServer()
{
	m_func_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (m_func_result != 0)
		return false;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	m_func_result = getaddrinfo(0, DEFAULT_PORT, &hints, &m_host_info);
	if (m_func_result != 0)
	{
		WSACleanup();
		return false;
	}
	return true;
}

bool SocketServer::CreateListeningSocket()
{
	m_listen_socket = socket(m_host_info->ai_family, m_host_info->ai_socktype, m_host_info->ai_protocol);
	if (m_listen_socket == INVALID_SOCKET)
	{
		freeaddrinfo(m_host_info);
		WSACleanup();
		return false;
	}
	return true;
}

bool SocketServer::StartListening()
{
	m_func_result = bind(m_listen_socket, m_host_info->ai_addr, (int)m_host_info->ai_addrlen);
	if (m_func_result == SOCKET_ERROR)
	{
		freeaddrinfo(m_host_info);
		closesocket(m_listen_socket);
		WSACleanup();
		return false;
	}

	freeaddrinfo(m_host_info);

	m_func_result = listen(m_listen_socket, SOMAXCONN);
	if (m_func_result == SOCKET_ERROR)
	{
		closesocket(m_listen_socket);
		WSACleanup();
		return false;
	}
	return true;
}
bool SocketServer::AcceptConnection()
{
	m_client_socket = accept(m_listen_socket, NULL, NULL);
	if (m_client_socket == INVALID_SOCKET)
	{
		closesocket(m_listen_socket);
		WSACleanup();
		return false;
	}
	return true;
}
int  SocketServer::GetMessageLength()
{
	std::vector<char> bytes_number;
	bytes_number.resize(DEFAULT_BUFLEN);
	recv(m_client_socket, &bytes_number[0], bytes_number.size(), 0);

	return atoi(bytes_number.data());
}
bool SocketServer::ReceiveMessage()
{
	int total_bytes_count = GetMessageLength();
	int recived_bytes_count = 0;

	do
	{
		m_buffer.clear();
		m_buffer.resize(DEFAULT_BUFLEN);
		m_func_result = recv(m_client_socket, &m_buffer[0], m_buffer.size(), 0);
		if (m_func_result > 0)
		{
			std::cout << "Bytes received: " << m_func_result << "\n";
			recived_bytes_count += m_func_result;
		}
		else if (m_func_result == 0)
			std::cout << "Connection closing...\n";
		else
		{
			closesocket(m_client_socket);
			WSACleanup();
			return false;
		}
	} while (recived_bytes_count < total_bytes_count);

	std::cout << "Total bytes received: " << recived_bytes_count << "\n";
	return true;
}
bool SocketServer::ShutdownServer()
{
	m_func_result = shutdown(m_client_socket, SD_SEND);
	if (m_func_result == SOCKET_ERROR)
	{
		closesocket(m_client_socket);
		WSACleanup();
		return false;
	}

	closesocket(m_client_socket);
	closesocket(m_listen_socket);
	WSACleanup();

	return true;
}



