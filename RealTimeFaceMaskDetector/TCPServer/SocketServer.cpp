#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SocketServer.h"
#include "Ws2spi.h"

bool SocketServer::InitSocketServer()
{
	LOG_MSG << "InitSocketServer: begin";
	ConnectToSQL();

	std::unique_ptr<ConfigReader>parser = std::make_unique<XMLParser>(CONFIG_FILE);

	m_ip = parser->GetParam("Client", "ip");
	m_port = parser->GetParam("Client", "port");
	m_func_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (m_func_result != 0)
	{
		LOG_ERROR << "InitSocketServer: WSAStartup: ERROR " << GetLastError();
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	m_func_result = getaddrinfo(0, m_port.c_str(), &hints, &m_host_info);
	if (m_func_result != 0)
	{
		WSACleanup();
		LOG_ERROR << "InitSocketServer: getaddrinfo: ERROR " << GetLastError();
		return false;
	}

	SpecifyPathForPhotos();
	LOG_MSG << "InitSocketServer: cuccess!";
	return true;
}

bool SocketServer::CreateListeningSocket()
{
	m_listen_socket = socket(m_host_info->ai_family, m_host_info->ai_socktype, m_host_info->ai_protocol);

	if (m_listen_socket == INVALID_SOCKET)
	{
		LOG_ERROR << "CreateListeningSocket: socket: ERROR " << GetLastError();
		freeaddrinfo(m_host_info);
		WSACleanup();
		return false;
	}
	server_is_up = true;
	LOG_MSG << "CreateListeningSocket: created!";
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
		LOG_ERROR << "BindListeningSocket: bind: ERROR " << GetLastError();
		return false;
	}
	LOG_MSG << "BindListeningSocket: binded!";
	return true;
}

void SocketServer::MakeAccept()
{
	SOCKET mock_socket = INVALID_SOCKET;
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(std::stoi(m_port));

	mock_socket = socket(m_host_info->ai_family, m_host_info->ai_socktype,
		m_host_info->ai_protocol);
	connect(mock_socket, (SOCKADDR*)&clientService, sizeof(clientService));
	closesocket(mock_socket);
}

bool SocketServer::AcceptConnection()
{
	std::cout << "Waiting connection" << std::endl;
	bool ready = false, stop = false;
	std::future<SOCKET> cl_socket = std::async(std::launch::async, [this, &ready]
	{
		SOCKET result;
		result = accept(m_listen_socket, NULL, NULL);
		ready = true;
		return result;
	});
	std::cout << "Waiting...     Press \'ESC\' to stop the Server\n" << std::flush;
	bool key = { false };
	while (!ready)
	{
		key = GetAsyncKeyState(VK_ESCAPE);
		if (key)
		{
			server_is_up = false;
			MakeAccept();
			stop = true;
		}
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
		WSACleanup();
		sql_server->Disconnect();
		return false;
	}
	LOG_MSG << "AcceptConnection: accepted!";
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
	LOG_MSG << "StartListening: Server work begin...";
	while (server_is_up)
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
			LOG_ERROR << "StartListening: listen: failed to listen socket";
			closesocket(m_listen_socket);//ask to delete
			WSACleanup();//ask to delete
			ret_value = false;
			break;
		}
	}
	LOG_MSG << "StartListening: Server work end!";
	return ret_value;
}

int  SocketServer::GetMessageLength()
{
	std::vector<char> bytes_number;
	bytes_number.resize(DEFAULT_BUFLEN);
	recv(m_client_socket, &bytes_number[0], bytes_number.size(), 0);
	return atoi(bytes_number.data());
}

bool SocketServer::ReceiveFullMessage()
{
	recv_mutex.lock();
	size_t total_bytes_count = GetMessageLength();

	if (total_bytes_count == 0)
	{
		LOG_WARNING << "ReceiveFullMessage: GetMessageLength: incorrect message length";
		recv_mutex.unlock();
		return false;
	}

	m_buffer.resize(total_bytes_count + 1);
	m_func_result = recv(m_client_socket, &m_buffer[0], m_buffer.size(), 0);
	recv_mutex.unlock();
	if (m_func_result > 0) // correctrly receided message
	{
		LOG_MSG << "Total bytes: " << total_bytes_count << " Received: " << m_func_result;
	}
	else if (m_func_result == 0) // client closed connection
	{
		LOG_MSG << "Connection closing...";
		return false;
	}
	else // error when receiving message. <Check possible errors in documentation for 'recv' function>
	{
		closesocket(m_client_socket);//ask to delete
		WSACleanup();//ask to delete
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
	LOG_MSG << "ReceiveMessage: begin: work with client";
	ret_value = true;
	bool client_connected = true;

	while (client_connected)
	{

		m_buffer.clear();
		try
		{
			TryReceiveAndSendMessage(client_connected);

		}
		catch (const std::string& msg)
		{
			LOG_ERROR << msg;
			ret_value = false;
			break;
		}
	}
	LOG_MSG << "ReceiveMessage: end: work with client";
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
	LOG_MSG << "SaveAndSendData: writing photo to file...";
	recv_data.write(m_buffer.data(), m_buffer.size());
	recv_data.close();
	recv_mutex.unlock();

	//Writing in database
	//here must be decrypt
	UpdateDataBase();
}

bool SocketServer::UpdateDataBase()
{
	LOG_MSG << "UpdateDataBase: updating databse...";
	try
	{
		std::lock_guard<std::mutex> lock(sql_mutex);
		sql_server->InsertPhoto(m_photo_to_send);
		LOG_MSG << "UpdateDataBase: database updated";
	}
	catch (const SQLException& e)
	{
		LOG_WARNING << e.what();
	}

	return true;
}

void SocketServer::CreateTableIfNeeded(std::unique_ptr<SQLConnection>& sql_server)
{
	if (!sql_server->CheckTableExists())
	{
		LOG_MSG << "CreateTableIfNeeded: table doesn't exist. Creating new one...";
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

SocketServer::~SocketServer()
{
	ShutdownServer();
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
		LOG_WARNING << "OpenParticularFile: photo_path doesn't exist";
		return false;
	}
	stream.open(photo_path, std::ios::binary);
	if (!stream.is_open())
	{
		LOG_WARNING << "OpenParticularFile: ofstream:open: fail to open";
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
		LOG_MSG << "ConnectToSQL: begin";
		sql_server->GetIniParams(CONFIG_FILE);

		sql_server->Connect();
		LOG_MSG << "ConnectToSQL: connected!";
		CreateTableIfNeeded(sql_server);
		LOG_MSG << "ConnectToSQL: end";
	}
	catch (const SQLException& e)
	{
		LOG_ERROR << e.what();
	}
}