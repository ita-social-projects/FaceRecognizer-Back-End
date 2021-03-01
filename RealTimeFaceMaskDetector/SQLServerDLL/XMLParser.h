#pragma once

#include "ConfigReader.h"

class CONFIGREADER_API XMLParser : public ConfigReader
{
public:
	XMLParser(std::string ini);

	void ReadFile(std::string filename);

	std::string GetParam(const std::string& section, 
		const std::string& param);
private:
	/*Parse records from ini file*/
	void Parse();
	std::vector<Section> contents;
	std::string inipath;
};