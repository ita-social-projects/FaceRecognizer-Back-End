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

bool SocketServer::SendMessage()
{
	// Connection information:
		std::string server_name{ "SAPIK\\SQLEXPRESS" };                     // "" if server exists on your local machine
		std::string database_name{ "MaskPhotosDatabase" };
		std::string username{ "" };                          // "" if Windows authentification
		std::string password{ "" };                          // "" if Windows authentification
		std::string database_string = server_name + "@" + database_name; // 1-st parameter of 'Connect' method

		SQLServer sql_server;
		std::cout << database_string << std::endl;
		try
		{
			// -- Connect --
			sql_server.Connect(database_string, username, password);

			// -- Insert photo --
			std::string photoPath{ R"(Photos)" };
			std::string photoName{ "Avatar" };
			std::string photoExtension{ "png" };
			sql_server.InsertPhoto(photoPath, photoName, photoExtension);

			// -- Disconnect --
			sql_server.Disconnect();
		}
		catch (const SAException& ex)
		{
			sql_server.RollBack();
			std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		}

		return true;
}

bool SocketServer::ReceiveMessage()
{
	std::ofstream recv_data;
	recv_data.open("Avatar.png", std::ios::binary);
	if (!recv_data.is_open())
		return false;

	int total_bytes_count = GetMessageLength();
	int recived_bytes_count = 0;
	std::vector<char>temp_buffer{};

	do
	{
		temp_buffer.clear();
		temp_buffer.resize(DEFAULT_BUFLEN);
		m_func_result = recv(m_client_socket, &temp_buffer[0], temp_buffer.size(), 0);
		if (m_func_result > 0)
		{
			recived_bytes_count += m_func_result;
			m_buffer.insert(end(m_buffer), begin(temp_buffer), end(temp_buffer));
		}
		else if (m_func_result == 0)
		{
			LOG_MSG << "Connection closing...";
		}
		else
		{
			closesocket(m_client_socket);
			WSACleanup();
			return false;
		}
	} while (recived_bytes_count < total_bytes_count);

	recv_data.write(m_buffer.data(), m_buffer.size());
	recv_data.close(); 

	EncryptDecryptAES_ECBMode decryptor;
	std::string data;
	std::string cipher(m_buffer.begin(), m_buffer.end());
	decryptor.Decrypt(cipher, data);

	LOG_MSG << "Total bytes received: " << recived_bytes_count;
	SendMessage();
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