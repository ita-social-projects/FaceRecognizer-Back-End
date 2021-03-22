#include "pch.h"
#include <utility>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "IniParser.h"

IniParser::IniParser(std::string ini) :inipath(ini)
{
    Parse();
}

void IniParser::Parse()
{
    contents.clear();
    if (!std::filesystem::exists(inipath))
    {
        std::cout << "Cannot find the config file" << std::endl;
        return;
    }
    std::string temp;
    std::ifstream file(inipath);
    std::string section;
    Params temp_param;
    Section temp_section;
    for (std::string text; getline(file, text); )
    {
        text.erase(remove(text.begin(), text.end(), ' '), text.end());
        text.erase(std::remove(text.begin(), text.end(), '\t'), text.end());
        if (text.empty())
        {
            continue;
        }
        if (text.find(";") == 0)
        {
            continue;
        }
        if (text.find("[") == 0)
        {
            size_t last = text.find(']');
            
            if (temp_section.params.size()>0)
            {
                contents.push_back(temp_section);
            }
            temp_section.name = text.substr(1, last - 1);
            temp_section.params.clear();
            continue;
        }
        if (text.find("=") > 0)
        {
            temp_param.param = text.substr(0, text.find('='));
            temp_param.value = text.substr(text.find('=') + 1, text.length() - 1);
            if (temp_param.value == "\"\"")
            {
                temp_param.value = "";
            }
            temp_section.params.push_back(temp_param);
            continue;
        }
    }
    if (temp_section.params.size() > 0)
    {
        contents.push_back(temp_section);
    }
    file.close();
}

std::string IniParser::GetParam(const std::string& section, const std::string& param)
{
    for (size_t i = 0; i != contents.size(); i++)
    {
        if (contents[i].name == section)
        {
            for (size_t j = 0; j != contents[i].params.size(); j++)
            {
                if (param == contents[i].params[j].param)
                {
                    return contents[i].params[j].value;
                }
            }
        }
    }
    return "No such parameter";
}

void IniParser::ReadFile(std::string filename)
{
    inipath = filename;
}