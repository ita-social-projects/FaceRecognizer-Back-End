#include "pch.h"
#include "Logger.h"
INITIALIZE_EASYLOGGINGPP
el::Configurations Logger::s_configuration{};
el::Level Logger::s_level{ el::Level::Global };
std::string Logger::s_format{};
std::string Logger::s_filename{};
long long unsigned Logger::s_max_log_file_size{};
size_t Logger::s_logs_flush_thredsold{};
int Logger::s_subsecond_precision{};
bool Logger::s_enabled{};
bool Logger::s_to_file{};
bool Logger::s_to_standart_output{};
bool Logger::s_performance_tracking{};
bool Logger::s_is_configurated = false;

bool Logger::SetConfiguration() {
	if (!s_is_configurated)
	{
		std::filesystem::create_directory(LOG_FOLDER);

		ReadConfiguration();
		s_configuration.setGlobally(el::ConfigurationType::Format, s_format);
		s_configuration.setGlobally(el::ConfigurationType::Filename, s_filename);
		s_configuration.setGlobally(el::ConfigurationType::LogFlushThreshold, std::to_string(s_logs_flush_thredsold));
		s_configuration.setGlobally(el::ConfigurationType::Enabled, std::to_string(s_enabled));
		s_configuration.setGlobally(el::ConfigurationType::ToFile, std::to_string(s_to_file));
		s_configuration.setGlobally(el::ConfigurationType::ToStandardOutput, std::to_string(s_to_standart_output));
		s_configuration.setGlobally(el::ConfigurationType::SubsecondPrecision, std::to_string(s_subsecond_precision));
		s_configuration.setGlobally(el::ConfigurationType::PerformanceTracking, std::to_string(s_performance_tracking));
		s_configuration.setGlobally(el::ConfigurationType::MaxLogFileSize, std::to_string(s_max_log_file_size));

		el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
		el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

		el::Helpers::installPreRollOutCallback(Logger::LogsRollout);

		el::Loggers::reconfigureAllLoggers(s_configuration);		

		s_is_configurated = true;
	}
	return s_is_configurated;
}

bool Logger::SetConfiguration(std::string& conf_filename)
{
	if (!s_is_configurated)
	{
		std::filesystem::create_directory(LOG_FOLDER);

		ReadConfiguration(conf_filename);
		s_configuration.setGlobally(el::ConfigurationType::Format, s_format);
		s_configuration.setGlobally(el::ConfigurationType::Filename, s_filename);
		s_configuration.setGlobally(el::ConfigurationType::LogFlushThreshold, std::to_string(s_logs_flush_thredsold));
		s_configuration.setGlobally(el::ConfigurationType::Enabled, std::to_string(s_enabled));
		s_configuration.setGlobally(el::ConfigurationType::ToFile, std::to_string(s_to_file));
		s_configuration.setGlobally(el::ConfigurationType::ToStandardOutput, std::to_string(s_to_standart_output));
		s_configuration.setGlobally(el::ConfigurationType::SubsecondPrecision, std::to_string(s_subsecond_precision));
		s_configuration.setGlobally(el::ConfigurationType::PerformanceTracking, std::to_string(s_performance_tracking));
		s_configuration.setGlobally(el::ConfigurationType::MaxLogFileSize, std::to_string(s_max_log_file_size));

		el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
		el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

		el::Helpers::installPreRollOutCallback(Logger::LogsRollout);

		el::Loggers::reconfigureAllLoggers(s_configuration);

		s_is_configurated = true;
	}
	return s_is_configurated;
}

bool Logger::ReadConfiguration()
{
	el::Logger* logger{ el::Loggers::getLogger(LOGGER_ID) };
	logger->configure(CONFIGURATION_PATH);

	s_format = logger->typedConfigurations()->logFormat(s_level).format();
	s_filename = logger->typedConfigurations()->filename(s_level);
	s_logs_flush_thredsold = logger->typedConfigurations()->logFlushThreshold(s_level);
	s_subsecond_precision = logger->typedConfigurations()->subsecondPrecision(s_level).m_width;
	s_max_log_file_size = logger->typedConfigurations()->maxLogFileSize(s_level);
	s_enabled = logger->typedConfigurations()->enabled(s_level);
	s_to_file = logger->typedConfigurations()->toFile(s_level);
	s_to_standart_output = logger->typedConfigurations()->toStandardOutput(s_level);
	s_performance_tracking = logger->typedConfigurations()->performanceTracking(s_level);

	return s_is_configurated;
}

bool Logger::ReadConfiguration(const std::string& conf_filename)
{
	el::Logger* logger{ el::Loggers::getLogger(LOGGER_ID) };
	logger->configure(conf_filename);

	s_format = logger->typedConfigurations()->logFormat(s_level).format();
	s_filename = logger->typedConfigurations()->filename(s_level);
	s_logs_flush_thredsold = logger->typedConfigurations()->logFlushThreshold(s_level);
	s_subsecond_precision = logger->typedConfigurations()->subsecondPrecision(s_level).m_width;
	s_max_log_file_size = logger->typedConfigurations()->maxLogFileSize(s_level);
	s_enabled = logger->typedConfigurations()->enabled(s_level);
	s_to_file = logger->typedConfigurations()->toFile(s_level);
	s_to_standart_output = logger->typedConfigurations()->toStandardOutput(s_level);
	s_performance_tracking = logger->typedConfigurations()->performanceTracking(s_level);

	return s_is_configurated;
}

bool Logger::LogsRollout(const char* file_name, size_t file_size)
{	
	return !rename(file_name, Logger::CreateLogFileName().c_str());
}

std::string Logger::CreateLogFileName()
{
	std::string new_file_name = LOG_FOLDER.string() + LOG_NAME + " (" + std::to_string(Logger::FileCount()) + ")" + LOG_EXTENSION;
	return new_file_name;
}

int Logger::FileCount()
{
	int file_counter{};
	for (const auto& entry : std::filesystem::directory_iterator(LOG_FOLDER)) ++file_counter;
	return file_counter;
}