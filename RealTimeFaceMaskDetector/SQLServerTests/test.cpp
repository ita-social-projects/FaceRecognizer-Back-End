#include "pch.h"
#include "../SQLServerDLL/IniParser.cpp"
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
