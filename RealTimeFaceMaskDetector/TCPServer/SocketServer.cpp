#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SocketServer.h"
#include "Ws2spi.h"

bool SocketServer::InitSocketServer()
{
	ConnectToSQL();
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

	SpecifyPathForPhotos();
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

bool SocketServer::BindListeningSocket()
{
	m_func_result = bind(m_listen_socket, m_host_info->ai_addr, (int)m_host_info->ai_addrlen);
	if (m_func_result == SOCKET_ERROR)
	{
		freeaddrinfo(m_host_info);
		closesocket(m_listen_socket);
		WSACleanup();
		LOG_ERROR << "BindListeningSocket ERROR: faild to bind socket";
		LOG_ERROR << GetLastError();
		return false;
	}
	return true;
}

bool SocketServer::AcceptConnection()
{
	std::cout << "Waiting connection" << std::endl;
	//Here make async wait
	bool ready = false, stop = false;
	SOCKET mock_socket = INVALID_SOCKET;
	std::future<SOCKET> cl_socket = std::async(std::launch::async, [this,&ready] 
		{ 
			SOCKET result;

			result = accept(m_listen_socket, NULL, NULL);

			ready = true;
			return result;
		});
	
	std::cout << "Waiting...     Press \'ESC\' to stop the Server\n" << std::flush;

	bool key = { false };
	bool old_key = { false };
	while (!ready)
	{
		//wait
		key = GetAsyncKeyState(VK_ESCAPE) & 0x01;
		if (key && !old_key)
		{
			server_is_up = false;

			sockaddr_in clientService;
			clientService.sin_family = AF_INET;
			clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
			clientService.sin_port = htons(27015);
			
			mock_socket = socket(m_host_info->ai_family, m_host_info->ai_socktype, m_host_info->ai_protocol);
			connect(mock_socket, (SOCKADDR*)&clientService, sizeof(clientService));
			stop = true;
		}

		old_key= key;
	}
	m_client_socket = cl_socket.get();
	
	if (m_client_socket == INVALID_SOCKET)
	{	
		std::cout << "INVALID_SOCKET" << std::endl;
		closesocket(m_listen_socket);
		WSACleanup();
		LOG_WARNING << "AcceptConnection: failed to accept client";
		return false;
	}
	if (stop)
	{
		closesocket(m_listen_socket);
		closesocket(mock_socket);
		WSACleanup();
		sql_server->Disconnect();
		return false;
	}
	return true;
}

void SocketServer::TryAcceptAndStartMessaging(bool& ret_value)
{
	if (AcceptConnection())
	{	
		std::cout << "Begin..." << std::endl;
		StartMessagingWintClient(ret_value);	
	}
	else
	{
		std::cout << "Stopped..." << std::endl;
		ret_value = false;
	}
}

void SocketServer::StartMessagingWintClient(bool& ret_value)
{
	std::thread th = std::thread([&]() 
		{
			ReceiveMessage(ret_value); 
		});
	if (th.joinable()) 
	{
		th.join();
	}
	std::cout << "Stopped connection" << std::endl;
}

bool SocketServer::StartListening(bool& ret_value)
{
	ret_value = BindListeningSocket();
	if (!ret_value)
		return ret_value;

	freeaddrinfo(m_host_info);

	while(server_is_up)
	{
		if (listen(m_listen_socket, SOMAXCONN) != SOCKET_ERROR)
		{	
			TryAcceptAndStartMessaging(ret_value);
			if (!ret_value)
			{
				std::cout << "Exiting..." << std::endl;
				break;
			}
		}
		else 
		{
			LOG_ERROR << "StartListening: failed to listen socket";
			closesocket(m_listen_socket);
			WSACleanup();
			ret_value = false;
			break;
		}
	}
	
	return ret_value;
}

int  SocketServer::GetMessageLength()
{
	std::vector<char> bytes_number;
	bytes_number.resize(DEFAULT_BUFLEN);
	recv_mutex.lock();
	recv(m_client_socket, &bytes_number[0], bytes_number.size(), 0);
	return atoi(bytes_number.data());
}

bool SocketServer::ReceiveFullMessage()
{
	int total_bytes_count = GetMessageLength();

	if (total_bytes_count == 0)
	{
		recv_mutex.unlock();
		return false;
	}

	m_buffer.resize(total_bytes_count + 1);
	m_func_result = recv(m_client_socket, &m_buffer[0], m_buffer.size(), 0);
	recv_mutex.unlock();
	if (m_func_result > 0) // correctrly receided message
	{
		LOG_MSG << "Total bytes: "<< total_bytes_count <<" Received: " << m_func_result;
	}
	else if (m_func_result == 0) // client closed connection
	{
		LOG_MSG << "Connection closing...";
		return false;
	}
	else // error when receiving message. <Check possible errors in documentation for 'recv' function>
	{
		closesocket(m_client_socket);
		WSACleanup();
		throw std::string("Receive ERROR");
	}
	return true;
}

void SocketServer::TryReceiveAndSendMessage(bool& client_connected)
{
	if (ReceiveFullMessage())
	{
		SaveAndSendData();
	}
	else
	{
		client_connected = false;
	}
}

bool SocketServer::ReceiveMessage(bool& ret_value)
{	
	ret_value = true;
	bool client_connected = true;

	while (client_connected)
	{
		
		m_buffer.clear();
		try
		{
			TryReceiveAndSendMessage(client_connected);
			
		}
		catch(const std::string& msg)
		{
			LOG_ERROR << msg;
			ret_value = false;
			break;
		}
	}
	return ret_value;
}

void SocketServer::SaveAndSendData()
{
	std::ofstream recv_data;

	if (!OpenParticularFile(recv_data))
	{
		return;
	}
	recv_mutex.lock();
	recv_data.write(m_buffer.data(), m_buffer.size());
	recv_data.close();
	recv_mutex.unlock();

	//Writing in database
	//here must be decrypt
	UpdateDataBase();
}

bool SocketServer::UpdateDataBase()
{
	try
	{
		std::lock_guard<std::mutex> lock(sql_mutex);
		sql_server->InsertPhoto(m_photo_to_send);
	}
	catch (const SQLException& e)
	{
		std::cout << e.what() << std::endl;
	}

	return true;
}

void SocketServer::CreateTableIfNeeded(std::unique_ptr<SQLConnection>& sql_server)
{
	if (!sql_server->CheckTableExists())
	{
		sql_server->CreatePhotosTable();
	}
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
	try
	{
		sql_server->Disconnect();
	}
	catch (const SQLException& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return true;
}

/*Functions for making directory for photos, 
and creating particular name for each photo,
containing date and time
*/
std::filesystem::path SocketServer::GetCurrentPath()
{
	wchar_t path[MAX_PATH];
	GetModuleFileName(nullptr, path, MAX_PATH);
	std::filesystem::path current_directory(path);
	current_directory.remove_filename();

	return current_directory;
}

bool SocketServer::SpecifyPathForPhotos()
{
	std::filesystem::path current_directory = GetCurrentPath();
	current_directory += "images";

	return std::filesystem::create_directory(current_directory);
}

bool SocketServer::OpenParticularFile(std::ofstream& stream)
{
	std::string file_creation_date;
	CreateFileNameSpecificator(file_creation_date);
	m_photo_to_send.date = file_creation_date;
	/*Date format contains symbols like ' ' and ':'.
	Replace them to avoid forbidden symbols in filename */
	for (auto& symbol : file_creation_date)
	{
		ReplaceForbiddenSymbol(symbol);
	}
	std::filesystem::path photos_directory = GetCurrentPath() / "images";
	m_photo_to_send.path = photos_directory.string() + "\\";
	m_photo_to_send.name = "Photo_" + file_creation_date;
	m_photo_to_send.extension = "png";

	std::string photo_path{ m_photo_to_send.path +
							m_photo_to_send.name + "." +
							m_photo_to_send.extension };
	
	if (std::filesystem::exists(photo_path))
	{
		return false;
	}
	stream.open(photo_path, std::ios::binary);
	if (!stream.is_open())
	{
		return false;
	}

	return true;
}

void SocketServer::CreateFileNameSpecificator(std::string& file_specificator)
{
	time_t curent_time;
	time(&curent_time);
	tm current_date;
	localtime_s(&current_date, &curent_time);

	/*converting date to string*/
	char str[50];
	strftime(str, 50, "%Y-%m-%d %H:%M:%S", &current_date);
	file_specificator = str;
}

void SocketServer::ReplaceForbiddenSymbol(char& symbol)
{
	if (symbol == ' ' || symbol == ':')
	{
		symbol = '_';
	}
}

void SocketServer::ConnectToSQL()
{
	sql_server = std::make_unique<SQLServer>();
	try
	{
		sql_server->GetIniParams(CONFIG_FILE);

		sql_server->Connect();
		CreateTableIfNeeded(sql_server);
	}
	catch (const SQLException& e)
	{
		std::cout << e.what() << std::endl;
	}
}