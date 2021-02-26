#pragma once
#ifdef SQLSERVER_EXPORTS
#define INIPARSER_API __declspec(dllexport)
#else
#define INIPARSER_API __declspec(dllimport)
#endif

struct Params
{
	std::string param;
	std::string value;
};

struct Section
{
	std::string name;
	std::vector<Params> params;
};

class INIPARSER_API ConfigReader abstract
{
public:
	/*Get the name of the config file*/
	virtual void ReadFile(std::string filename)=0;
	/*Get value of the parameter with the given section and parameter name*/
	virtual std::string GetParam(const std::string& section, const std::string& param)=0;
};