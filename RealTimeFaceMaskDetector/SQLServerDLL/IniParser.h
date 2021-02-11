#pragma once
#include <utility>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

#ifdef SQLSERVER_EXPORTS
#define INIPARSER_API __declspec(dllexport)
#else
#define INIPARSER_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;

class INIPARSER_API IniParser
{
public:
	IniParser(std::string ini);
	/*Parse records from ini file*/
	void Parse();
	/*Get value of the parameter with the given section and parameter name*/
	std::string GetParam(const std::string& section, const std::string& param);
private:
	std::vector<std::pair<std::string,std::vector<std::pair<std::string, std::string>>>> contents;
	fs::directory_entry inipath;
	std::vector<std::string> sections;
};
