#include "SocketServer.h"

bool SocketServer::InitSocketServer()
{
	m_func_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (m_func_result != 0)
	{
		return false;
	}

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

	m_buffer.resize(DEFAULT_BUFLEN);

	if (!SpecifyPathForPhotos())
	{
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

	server_is_up = true;

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
	std::string server_name{ "SSU-SQL" };                     // "" if server exists on your local machine
	std::string database_name{ "MaskPhotosDatabase" };
	std::string username{ "Lv-535.DB" };                          // "" if Windows authentification
	std::string password{ "Lv-535.DB" };                          // "" if Windows authentification
	std::string database_string = server_name + "@" + database_name; // 1-st parameter of 'Connect' method

	SQLServer sql_server;
	std::cout << database_string << std::endl;
	try
	{
		// -- Connect --
		sql_server.Connect(database_string, username, password);

		// -- Insert photo --
		std::string photoPath{ R"(D:\Programing\Real-Time-Face-Mask-Detector-Server\RealTimeFaceMaskDetector\x64\Debug\images\)" };
		std::string photoName{ "Avatar" + file_specificator };
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

bool SocketServer::SpecifyPathForPhotos()
{
	std::filesystem::path current_directory = GetCurrentPath();
	current_directory += "images";

	return std::filesystem::create_directory(current_directory);
}

bool SocketServer::OpenParticularFile(std::ofstream& stream)
{
	/*file_spesificator variable vill be initialised here*/
	CreateFileNameSpecificator();
	
	std::filesystem::path current_directory = GetCurrentPath();
	
	std::string photo_path{ current_directory.string() + "images\\Avatar_" + file_specificator + ".png" };
	stream.open(photo_path, std::ios::binary);
	if (!stream.is_open())
		return false;

	return true;
}

void SocketServer::CreateFileNameSpecificator()
{
	time_t curent_time;
	time(&curent_time);
	tm current_date;
	localtime_s(&current_date, &curent_time);
	char str[50];
	asctime_s(str, 50, &current_date);
	file_specificator = str;
	file_specificator.erase(file_specificator.size() - 1);

	for(auto& symbol : file_specificator)
	{
		if(symbol == ' ')
		{
			symbol = '_';
		}
		else if(symbol == ':')
		{
			symbol = '.';
		}
	}
}

std::filesystem::path SocketServer::GetCurrentPath()
{
	wchar_t path[MAX_PATH];
	GetModuleFileName(nullptr, path, MAX_PATH);
	std::filesystem::path current_directory(path);
	current_directory.remove_filename();
	
	return current_directory;
}

bool SocketServer::ReceiveMessage()
{
	EncryptDecryptAES_ECBMode decryptor;

	/*change cycle to receive more than one photo*/
	std::ofstream recv_data;
	int recived_bytes_count = 0;
	while (server_is_up)
	{
		if(!OpenParticularFile(recv_data))
		{
			/*cannot open file error*/
		}

		int total_bytes_count = GetMessageLength();

		/*@FIX: The execution doesn't stop even when client is closed*/

		m_func_result = recv(m_client_socket, &m_buffer[0], m_buffer.size(), 0);

		if (m_func_result > 0)
		{
			recived_bytes_count = m_func_result;

			recv_data.write(m_buffer.data(), m_buffer.size());
			recv_data.close();
			std::string data;
			std::string cipher(m_buffer.begin(), m_buffer.end());
			decryptor.Decrypt(cipher, data);
			SendMessage();
		}
		else if (m_func_result == 0)
		{
			recv_data.close();
			LOG_MSG << "Connection closing...";
			break;
		}
		else
		{
			recv_data.close();
			closesocket(m_client_socket);
			WSACleanup();
			return false;
		}
		
		LOG_MSG << "Total bytes received: " << recived_bytes_count;
	}

	return true;
}

bool SocketServer::ShutdownServer()
{
	server_is_up = false;

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