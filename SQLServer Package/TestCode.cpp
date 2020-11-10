#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <SQLAPI.h>
#include <SQLServer.h>

int main()
{
	// Connection information:
	std::string server_name{ "YourServerName" };                     // "" if server exists on your local machine
	std::string database_name{ "YourDatabaseName" };
	std::string username{ "YourUsername" };                          // "" if Windows authentification
	std::string password{ "YourPassword" };                          // "" if Windows authentification
	std::string database_string = server_name + "@" + database_name; // 1-st parameter of 'Connect' method

	SQLServer sql_server;

	try
	{
		// -- Connect --
		sql_server.Connect(database_string, username, password);

		//// -- Insert photo --
		//std::string photoPath{ R"(D:\Downloads\)" };
		//std::string photoName{ "Avatar" };
		//std::string photoExtension{ "png" };
		//sqlServer.InsertPhoto(photoPath, photoName, photoExtension);

		//// -- Get all photos --
		//std::vector<SQLServer::PhotoType> photos = sqlServer.GetAllPhotos();
		//SQLServer::CreatePhotos(photos);

		//// -- Delete data from table --
		//sqlServer.ClearTable();

		//// -- Disconnect --
		//sqlServer.Disconnect();
	}
	catch (const SAException& ex)
	{
		sql_server.RollBack();
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}

	return 0;
}