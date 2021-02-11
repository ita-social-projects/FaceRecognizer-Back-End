#include "pch.h"
#include "IniParser.h"

IniParser::IniParser(std::string ini) :inipath(ini)
{
    Parse();
}

void IniParser::Parse()
{
    std::string temp;
    std::ifstream file(inipath);
    std::vector<std::pair<std::string, std::string>> params;
    params.clear();
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
            std::string strNew = text.substr(1, last - 1);
            sections.push_back(strNew);
            if (params.size() > 0)
            {
                contents.push_back(std::pair(temp, params));
            }
            params.clear();
            temp = strNew;
            continue;
        }
        if (text.find("=") > 0)
        {
            std::string left = text.substr(0, text.find('='));
            std::string right = text.substr(text.find('=') + 1, text.length() - 1);
            if (right == "\"\"")
            {
                right = "";
            }
            params.push_back(std::pair(left, right));
            continue;
        }
    }
    if (params.size() > 0)
    {
        contents.push_back(std::pair(temp, params));
    }
    file.close();
}

std::string IniParser::GetParam(const std::string& section, const std::string& param)
{
    for (size_t i = 0; i != sections.size(); i++)
    {
        if (sections[i] == section)
        {
            for (size_t j = 0; j != contents[i].second.size(); j++)
            {
                if (param == contents[i].second[j].first)
                {
                    return contents[i].second[j].second;
                }
            }
        }
    }
    return "No such parameter"; 
}