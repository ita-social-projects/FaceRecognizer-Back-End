#include "pch.h"
#include "SQLServer.h"
#include <fstream>

void SQLServer::SQLServerException::GetParams(const SAException& ex)
{
	err_message = ex.ErrText().GetMultiByteChars();
	err_code = ex.ErrNativeCode();
}

SQLServer::SQLServer(const ConnectParams& connect_string)
{
	try
	{
		Connect(connect_string);
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

void SQLServer::Connect(const ConnectParams& connect_string)
{
	try
	{
		Disconnect();
		std::string con = connect_string.server + "@" + connect_string.database;
		m_connection.Connect(_TSA(con.c_str()), _TSA(connect_string.username.c_str()), _TSA(connect_string.password.c_str()), SA_SQLServer_Client);
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

void SQLServer::Connect()
{
	try
	{
		Disconnect();
		std::string con = params.server + "@" + params.database;
		m_connection.Connect(_TSA(con.c_str()), _TSA(params.username.c_str()), _TSA(params.password.c_str()), SA_SQLServer_Client);
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

void SQLServer::InsertPhoto(const Photo& photo)
{
	try
	{
		std::string table_name{ "Photos" };
		std::string column_name_photo{ "Photo" };
		std::string column_name_photo_name{ "PhotoName" };
		std::string column_name_photo_extension{ "PhotoExtension" };
		std::string full_path = photo.path + photo.name + "." + photo.extension;

		std::string insert_statement = "INSERT INTO " + table_name;

		insert_statement +=
			" (" + column_name_photo +
			", " + column_name_photo_name +
			", " + column_name_photo_extension + ") ";

		insert_statement += "VALUES\n(\n(SELECT BulkColumn FROM OPENROWSET(BULK \'" + full_path + "\', SINGLE_BLOB) AS IMAGE),\n";
		insert_statement += "\'" + photo.name + "\',\n";
		insert_statement += "\'" + photo.extension + "\'\n);";

		SACommand insert(&m_connection, _TSA(insert_statement.c_str()));
		insert.Execute();
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

std::vector<SQLServer::PhotoType> SQLServer::GetAllPhotos()
{
	try
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
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

void SQLServer::RollBack()
{
	try
	{
		m_connection.Rollback();
	}
	catch (const SAException& ex)
	{
		sql_error.GetParams(ex);
		throw sql_error;
	}

}

void SQLServer::Disconnect()
{
	try
	{
		if (m_connection.isConnected())
			m_connection.Disconnect();
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}

}

void SQLServer::ClearTable(std::string table)
{
	try
	{
		SACommand delete_statement(&m_connection, _TSA(("DELETE FROM " + table).c_str()));
		delete_statement.Execute();
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}

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

void SQLServer::ExecSQLQuery(const std::string& query)
{
	try
	{
		SACommand mquery(&m_connection, _TSA(query.c_str()));
		mquery.Execute();
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

void SQLServer::DeleteRecord(int id)
{
	try
	{
		std::string query = "DELETE FROM PHOTOS WHERE ID=" + std::to_string(id);
		ExecSQLQuery(query);
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

bool SQLServer::CheckTableExists(std::string table)
{
	try
	{
		std::string query = "IF OBJECT_ID(\'" + table + "\', 'u') IS NOT NULL select 1 as \'result\' else select 0 as \'result\'";
		SACommand select(&m_connection, _TSA(query.c_str()));
		select.Execute();
		if (select.isResultSet())
		{
			select.FetchNext();
			return select.Field(_TSA("result")).asBool();
		}
	}
	catch (const SAException& ex)
	{
		RollBack();
		sql_error.GetParams(ex);
		throw sql_error;
	}
}

void SQLServer::GetIniParams(std::string path)
{
	IniParser parser(path);
	params.server = parser.GetParam("Server", "name");                    // "" if server exists on your local machine
	params.database = parser.GetParam("Server", "database");
	params.username = parser.GetParam("Server", "username");              // "" if Windows authentification
	params.password = parser.GetParam("Server", "password");              // "" if Windows authentification
}

std::string SQLServer::ExePath()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	std::wstring wpath = std::wstring(buffer).substr(0, pos);
	std::string result(wpath.begin(), wpath.end());
	return result;
}

void SQLServer::CreatePhotosTable(std::string table)
{
	std::ifstream file(ExePath() + "\\CreatePhotos.sql");

	std::string query((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	size_t index = 0;
	while (true) {
		/* Locate the substring to replace. */
		index = query.find("$TAB$", index);
		if (index == std::string::npos) break;

		/* Make the replacement. */
		query.replace(index, 5, table);

		/* Advance index forward so the next iteration doesn't pick it up as well. */
		index += 3;
	}
	ExecSQLQuery(query);
}