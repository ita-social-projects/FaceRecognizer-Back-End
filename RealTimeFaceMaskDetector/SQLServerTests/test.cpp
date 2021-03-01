#include "pch.h"
#include "../SQLServerDLL/IniParser.cpp"
#include "../SQLServerDLL/XMLParser.cpp"
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

TEST(TestXMLParser, TestParseXML)
{

	std::ofstream TestFile("TestXml.txt");

	TestFile << "<\?xml version=\"1.0\" encoding=\"UTF - 8\"\?\>" << "\n";
	TestFile << "<Config>" << "\n";
	TestFile << "<Server>" << "\n";
	TestFile << "<par1>val1</par1>" << "\n";
	TestFile << "<par2>val2</par2>" << "\n";
	TestFile << "<par3>val3</par3>" << "\n";
	TestFile << "</Server>" << "\n";
	TestFile << "<Client>" << "\n";
	TestFile << "<par1>val1</par1>" << "\n";
	TestFile << "<par2>val2</par2>" << "\n";
	TestFile << "</Client>" << "\n";
	TestFile << "</Config>" << "\n";

	TestFile.close();

	XMLParser parser("TestXml.txt");
	ASSERT_EQ("val1", parser.GetParam("Server", "par1"));
	ASSERT_EQ("val2", parser.GetParam("Server", "par2"));
	ASSERT_EQ("val3", parser.GetParam("Server", "par3"));
	ASSERT_EQ("val1", parser.GetParam("Client", "par1"));
	ASSERT_EQ("val2", parser.GetParam("Client", "par2"));
	ASSERT_EQ("No such parameter", parser.GetParam("Client", "par3"));
	remove("TestXml.txt");
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


