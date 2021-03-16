#ifndef LOG_SEARCHER_H
#define LOG_SEARCHER_H

#include <filesystem>
#include <vector>
#include <string>
#include <Windows.h>

namespace fs = std::filesystem;

class LogSearcher
{
public:
	using VectorString = std::vector<std::string>;
	static VectorString GetLogFilePaths()
	{
		VectorString paths_to_log_files;

		const std::string EXE_DIRECTORY = ExeDirectoryPath();
		const std::string LOG_DIRECTORY = EXE_DIRECTORY + "\\RTFMD_Logs";

		if (!fs::exists(LOG_DIRECTORY))
		{
			return {};
		}
		
		
		for (const auto& entry : fs::directory_iterator{ LOG_DIRECTORY })
		{
			if (entry.is_regular_file() && entry.path().has_extension() && entry.path().extension() == ".log")
			{
				paths_to_log_files.push_back(entry.path().u8string());
			}
		}
		
		return paths_to_log_files;
	}

private:
	static std::string ExeDirectoryPath() {
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		return std::string(buffer).substr(0, pos);
	}
};

#endif // LOG_SEARCHER_H