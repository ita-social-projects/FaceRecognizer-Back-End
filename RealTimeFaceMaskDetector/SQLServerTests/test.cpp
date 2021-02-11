#include "pch.h"
#include "../SQLServerDLL/IniParser.cpp"
#include "../SQLServerDLL/SQLServer.cpp"
#include "../SQLServerDLL/SQLException.cpp"
#include <fstream>

TEST(TestIniParser, TestParse) 
{

	std::ofstream TestFile("Test.txt");

	TestFile << "[Section1]"<<"\n";
	TestFile << "par1=val1"<<"\n";
	TestFile << "par2=val2" << "\n";
	TestFile << "[Section2]" << "\n";
	TestFile << "par3=val3" << "\n";

	TestFile.close();

	IniParser parser("Test.txt");
	ASSERT_EQ("val1", parser.GetParam("Section1","par1"));
	ASSERT_EQ("val2", parser.GetParam("Section1", "par2"));
	ASSERT_EQ("val3", parser.GetParam("Section2", "par3"));
	ASSERT_EQ("No such parameter", parser.GetParam("Section2", "par4"));
	remove("Test.txt");
}

TEST(TestSQLServer, TestCreateTable)
{
	std::shared_ptr<SQLConnection>sql_server = std::make_shared<SQLServer>();
	sql_server->GetIniParams(CONFIG_FILE);
	sql_server->Connect();
	if (!sql_server->CheckTableExists())
	{
		sql_server->CreatePhotosTable();
	}

	ASSERT_EQ(true, sql_server->CheckTableExists());
	sql_server->Disconnect();
}


