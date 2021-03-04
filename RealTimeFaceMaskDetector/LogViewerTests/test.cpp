#include "pch.h"
#include "../LogViewer/Parser.cpp"
#include "../LogViewer/structures.cpp"
#include "../LogViewer/Filter.cpp"
#include "../DateTimeDLL/DateTime.cpp"
#include <list>

const std::string path{ "C:/Users/Oleksii/Documents/Real-Time-Face-Mask-Detector-Server/TestData/testLog.log" };
void check_logs_by_options(size_t, std::vector<logger::LogOptions>);
void check_logs_by_time(size_t, const std::string&);
std::vector<logger::LogOptions> all_options{
		logger::LogOptions::INFO,
		logger::LogOptions::WARNING,
		logger::LogOptions::DEBUG,
		logger::LogOptions::ERR,
		logger::LogOptions::FATAL,
		logger::LogOptions::TRACE
};

TEST(LogViewerLogs, AllLogs) {
	constexpr int number_of_logs{ 12 };
	std::string from{ "0000-00-00 00:00:00" };
	std::string till{ "0000-00-00 00:00:00" };
	auto parser = logger::Parser(path);
	parser.parse(all_options, from, till);
	auto logs = parser.get_logs();

	EXPECT_EQ(logs.size(), number_of_logs);
}

TEST(LogViewerTests, InfoLogs) {
	constexpr int number_of_logs{ 4 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::INFO };
	check_logs_by_options(number_of_logs, options);
}


TEST(LogViewerTests, WarningLogs) {
	constexpr int number_of_logs{ 1 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::WARNING };
	check_logs_by_options(number_of_logs, options);
}

TEST(LogViewerTests, ErrorLogs) {
	constexpr int number_of_logs{ 2 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::ERR };
	check_logs_by_options(number_of_logs, options);
}

TEST(LogViewerTests, DebugLogs) {
	constexpr int number_of_logs{ 3 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::DEBUG };
	check_logs_by_options(number_of_logs, options);
}

TEST(LogViewerTests, FatalLogs) {
	constexpr int number_of_logs{ 2 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::FATAL };
	check_logs_by_options(number_of_logs, options);
}

TEST(LogViewerTests, InfoWarningLogs) {
	const int number_of_logs{ 5 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::INFO, logger::LogOptions::WARNING };
	check_logs_by_options(number_of_logs, options);
}

TEST(LogViewerTest, InfoErrorLogs) {
	const int number_of_logs{ 6 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::INFO, logger::LogOptions::ERR };
	check_logs_by_options(number_of_logs, options);
}

TEST(LogViewerTest, WarningErrorLogs) {
	const int number_of_logs{ 3 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::WARNING, logger::LogOptions::ERR };
	check_logs_by_options(number_of_logs, options);
}

void check_logs_by_options(size_t number_of_logs, std::vector<logger::LogOptions> options) {
	std::string datetime{ "0000-00-00 00:00:00" };
	auto parser = logger::Parser(path);
	parser.parse(options, datetime, datetime);
	auto logs = parser.get_logs();

	EXPECT_EQ(logs.size(), number_of_logs);
}

TEST(LogViewerTest, FromDatetime) {
	const size_t number_of_logs{ 8 };
	std::string from{ "2020-03-11 16:44:19" };
	std::string till{ "0000-00-00 00:00:00" };

	logger::Parser parser{ path };
	parser.parse(all_options, from, till);
	auto logs{ parser.get_logs() };

	EXPECT_EQ(logs.size(), number_of_logs);
}

TEST(LogViewerTest, TillDateTime) {
	const size_t number_of_logs{ 4 };
	std::string from{ "0000-00-00 00:00:00" };
	std::string till{ "2020-02-11 18:00:00" };

	logger::Parser parser{ path };
	parser.parse(all_options, from, till);
	auto logs{ parser.get_logs() };

	EXPECT_EQ(logs.size(), number_of_logs);
}
