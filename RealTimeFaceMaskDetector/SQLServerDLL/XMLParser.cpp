#include "pch.h"
#include <utility>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "XMLParser.h"

XMLParser::XMLParser(std::string ini) :inipath(ini)
{
    Parse();
}

void XMLParser::Parse()
{
    contents.clear();
    if (!std::filesystem::exists(inipath))
    {
        std::cout << "Cannot find the config file" << std::endl;
        return;
    }
    std::string temp;
    std::ifstream file(inipath);

    bool is_section = false;
    bool is_main_section = false;
    Params temp_param;
    Section temp_section;
    std::string main_section, section;
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
        if (text.find("<?xml")==0)
        {
            //beginning of the file
            continue;
        }
        if (text.find("<") == 0 && text.find("</") != 0)
        {
            //main section
            if (!is_main_section)
            {
                is_main_section = true;
                main_section = text.substr(text.find("<") + 1, text.find(">") - text.find("<") - 1);
                continue;
            }
            else
            {
                if (!is_section)
                {
                    //start of the new section
                    is_section = true;
                    section= text.substr(text.find("<") + 1, text.find(">") - text.find("<") - 1);
                    temp_section.name = section;
                    continue;
                }
                else
                {
                    //already in the section
                    temp_param.param = text.substr(text.find("<") + 1, text.find(">") - text.find("<") - 1);
                    temp_param.value = text.substr(text.find(">") + 1, text.find("</") - text.find(">") - 1);
                    if (temp_param.value == "\"\"")
                    {
                        temp_param.value = "";
                    }
                    temp_section.params.push_back(temp_param);
                    continue;
                }
            }
        }
        if (text.find("</") == 0)
        {
            //end of the section
            is_section = false;
            if (main_section == text.substr(text.find("</") + 1, text.find(">") - text.find("<") - 1))
            {
                //end of the main section
                is_main_section = false;
                continue;
            }
            else
            {
                contents.push_back(temp_section);
                temp_section.params.clear();
                continue;
            }
        }
    }
    file.close();
}

std::string XMLParser::GetParam(const std::string& section, const std::string& param)
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

void XMLParser::ReadFile(std::string filename)
{
    inipath = filename;
}