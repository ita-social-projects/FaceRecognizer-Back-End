#include "pch.h"
#include "SQLServer.h"
#include <fstream>

SQLServer::SQLServer(const std::string& database_string, const std::string& username, const std::string& password)
{
	Connect(database_string, username, password);
}

void SQLServer::Connect(const std::string& database_string, const std::string& username, const std::string& password)
{
	Disconnect();
	m_connection.Connect(_TSA(database_string.c_str()), _TSA(username.c_str()), _TSA(password.c_str()), SA_SQLServer_Client);
}

void SQLServer::InsertPhoto(const std::string& photo_path, const std::string& photo_name, const std::string& photo_extension)
{
	std::string table_name{ "Photos" };
	std::string column_name_photo{ "Photo" };
	std::string column_name_photo_name{ "PhotoName" };
	std::string column_name_photo_extension{ "PhotoExtension" };
	std::string full_path = photo_path + photo_name + "." + photo_extension;

	std::string insert_statement = "INSERT INTO " + table_name;

	insert_statement +=
		" (" + column_name_photo +
		", " + column_name_photo_name +
		", " + column_name_photo_extension + ") ";

	insert_statement += "VALUES\n(\n(SELECT BulkColumn FROM OPENROWSET(BULK \'" + full_path + "\', SINGLE_BLOB) AS IMAGE),\n";
	insert_statement += "\'" + photo_name + "\',\n";
	insert_statement += "\'" + photo_extension + "\'\n);";

	SACommand insert(&m_connection, _TSA(insert_statement.c_str()));

	insert.Execute();
}

std::vector<SQLServer::PhotoType> SQLServer::GetAllPhotos()
{
	std::string table_name{ "Photos" };
	std::string column_name_photo{ "Photo" };
	std::string column_name_photo_name{ "PhotoName" };
	std::string column_name_photo_extension{ "PhotoExtension" };

	std::string selectStatement =
		"SELECT " +
		column_name_photo + ", " +
		column_name_photo_name + ", " +
		column_name_photo_extension +
		" FROM " + table_name;

	SACommand select(&m_connection, _TSA(selectStatement.c_str()));

	select.Execute();

	std::vector<PhotoType> photos;

	if (select.isResultSet())
	{
		while (select.FetchNext())
		{
			PhotoType photo =
			{
				select.Field(_TSA(column_name_photo.c_str())).asLongBinary(),
				select.Field(_TSA(column_name_photo_name.c_str())).asString(),
				select.Field(_TSA(column_name_photo_extension.c_str())).asString()
			};

			photos.push_back(photo);
		}
	}

	return photos;
}

void SQLServer::RollBack()
{
	m_connection.Rollback();
}

void SQLServer::Disconnect()
{
	if (m_connection.isConnected())
		m_connection.Disconnect();
}

void SQLServer::ClearTable()
{
	std::string table_name{ "Photos" };

	SACommand delete_statement(&m_connection, _TSA(("DELETE FROM " + table_name).c_str()));

	delete_statement.Execute();
}

SQLServer::~SQLServer()
{
	Disconnect();
}

void SQLServer::CreatePhotos(const std::vector<PhotoType>& photos)
{
	for (const auto& photo : photos)
	{
		std::string fullname = std::string{ photo.photoName } + "." + std::string{ photo.photoExtension };

		std::ofstream fout(fullname, std::ios::binary);
		fout.write(photo.photoBytes, photo.photoBytes.GetLength());
		fout.close();
	}
}