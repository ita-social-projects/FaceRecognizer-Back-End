#pragma once

#include "ConfigReader.h"

class CONFIGREADER_API IniParser: public ConfigReader
{
public:
	IniParser(std::string ini);
	
	/*Get the name of the config file*/
	void ReadFile(std::string filename);

	/*Get value of the parameter with the given section and parameter name*/
	std::string GetParam(const std::string& section, const std::string& param);
private:
	/*Parse records from ini file*/
	void Parse();
	std::vector<Section> contents;
	std::string inipath;
};
