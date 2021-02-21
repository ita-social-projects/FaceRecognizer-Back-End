#include "pch.h"
#include "../LogViewer/Parser.cpp"

const std::string path{ "C:/Users/Oleksii/Documents/Real-Time-Face-Mask-Detector-Server/TestData/testLog.log" };
void check_logs(size_t, std::vector<logger::LogOptions>);

TEST(LogViewerLogs, AllLogs) {
	constexpr int number_of_logs{ 12 };
	auto parser = logger::Parser(path);
	auto logs = parser.get_logs();

	EXPECT_EQ(logs.size(), number_of_logs);
}

TEST(LogViewerTests, InfoLogs) {
	constexpr int number_of_logs{ 4 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::INFO };
	check_logs(number_of_logs, options);
}


TEST(LogViewerTests, WarningLogs) {
	constexpr int number_of_logs{ 1 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::WARNING };
	check_logs(number_of_logs, options);
}

TEST(LogViewerTests, ErrorLogs) {
	constexpr int number_of_logs{ 2 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::ERROR };
	check_logs(number_of_logs, options);
}

TEST(LogViewerTests, DebugLogs) {
	constexpr int number_of_logs{ 3 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::DEBUG };
	check_logs(number_of_logs, options);
}

TEST(LogViewerTests, FatalLogs) {
	constexpr int number_of_logs{ 2 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::FATAL };
	check_logs(number_of_logs, options);
}

TEST(LogViewerTests, InfoWarningLogs) {
	const int number_of_logs{ 5 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::INFO, logger::LogOptions::WARNING };
	check_logs(number_of_logs, options);
}

TEST(LogViewerTest, InfoErrorLogs) {
	const int number_of_logs{ 6 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::INFO, logger::LogOptions::ERROR };
	check_logs(number_of_logs, options);
}

TEST(LogViewerTest, WarningErrorLogs) {
	const int number_of_logs{ 3 };
	std::vector<logger::LogOptions> options{ logger::LogOptions::WARNING, logger::LogOptions::ERROR };
	check_logs(number_of_logs, options);
}

void check_logs(size_t number_of_logs, std::vector<logger::LogOptions> options) {
	auto parser = logger::Parser(path, options);
	auto logs = parser.get_logs();

	EXPECT_EQ(logs.size(), number_of_logs);
}