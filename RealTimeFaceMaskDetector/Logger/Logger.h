#ifndef LOGGER_H
#define LOGGER_H

#include "easylogging++.h"
#include <filesystem>

	const std::filesystem::path		LOG_FOLDER = R"(RTFMD_Logs\)";
	const std::string				CONFIGURATION_PATH = R"(Configuration.conf)";
	const std::string				LOG_NAME = static_cast<std::string>(__DATE__) + "_log";
	const std::string				LOG_EXTENSION = ".log";
	const std::string				LOGGER_ID = "RTFMD";

	// This class is exported from the dll
	class Logger {
	public:
		static bool					SetConfiguration();
		static bool					SetConfiguration(std::string& conf_filename);
	private:
		Logger() = default;
		Logger(const Logger&) = default;
		Logger(Logger&&) = default;
		Logger& operator=(const Logger&) = default;
		Logger& operator=(Logger&&) = default;

		static bool					LogsRollout(const char* file_name, size_t file_size);
		static bool					ReadConfiguration();
		static bool					ReadConfiguration(const std::string& conf_filename);
		static std::string			CreateLogFileName();
		static int					FileCount();

		static el::Configurations	s_configuration;
		static el::Level			s_level;
		static std::string			s_format;
		static std::string			s_filename;
		static long long unsigned	s_max_log_file_size;
		static size_t				s_logs_flush_thredsold;
		static int					s_subsecond_precision;
		static bool					s_enabled;
		static bool					s_to_file;
		static bool					s_to_standart_output;
		static bool					s_performance_tracking;
		static bool					s_is_configurated;
	};


// 1
#define LOG_MSG \
Logger::SetConfiguration(); \
LOG(INFO) 

#define LOG_WARNING \
Logger::SetConfiguration(); \
LOG(WARNING) 

#define LOG_ERROR \
Logger::SetConfiguration(); \
LOG(ERROR) 

// 2
#define LOG_DEBUG \
Logger::SetConfiguration(); \
LOG(DEBUG) 

// 3
#define LOG_TRACE \
Logger::SetConfiguration(); \
LOG(TRACE) 

#endif