#include "./Parser.h"
#include <iostream>

logger::Parser::Parser(const std::string& path) {
	open_file(path);
}

void logger::Parser::open_file(const std::string& path) {
	fs.open(path, std::ios_base::in);

	if (!fs) {
		// TODO: Exception
		std::cerr << "Could not open a file";
		return;
	}
}

void logger::Parser::parse
(
	const std::vector<LogOptions>& options, 
	const std::string& from, 
	const std::string& till
) 
{
	filter = Filter{ options, from, till };
	while (getline(fs, line)) {
		const bool match = parse_option() && parse_time();

		if (!match) {
			continue;
		}

		find_trace();
		find_message();
		logs.push_back(current_log);
	}
}

bool logger::Parser::parse_option() {
	find_type();
	if (filter.options_size() != number_of_options) {
		return filter.option_filter(current_log.option);
	}
	return true;
}

bool logger::Parser::parse_time() {
	find_time();
	return filter.from_datetime_filter(current_log.datetime) && filter.till_datetime_filter(current_log.datetime);
}

void logger::Parser::find_type() {
	if (std::regex_search(line, matches, option_pat)) {
		if (matches[0] == "[INFO]") {
			current_log.option = LogOptions::INFO;
		}
		else if (matches[0] == "[WARNING]") {
			current_log.option = LogOptions::WARNING;
		}
		else if (matches[0] == "[ERROR]") {
			current_log.option = LogOptions::ERR;
		}
		else if (matches[0] == "[DEBUG]") {
			current_log.option = LogOptions::DEBUG;
		}
		else if (matches[0] == "[FATAL]") {
			current_log.option = LogOptions::FATAL;
		}
		else if (matches[0] == "[TRACE]") {
			current_log.option = LogOptions::TRACE;
		}
	}
}

void logger::Parser::find_time() {
	std::smatch matches;
	if (!std::regex_search(line, matches, current_log.datetime.regex_pat())) {
		// TODO: throw exception
	}

	current_log.datetime = matches[0].str();
}

void logger::Parser::find_trace() {
	std::smatch match;
	
	if (std::regex_search(line, match, trace_pat)) {
		current_log.trace = match[0].str().substr(2);
	}
}

void logger::Parser::find_message() {
	std::smatch match;

	if (std::regex_search(line, match, message_pat)) {
		current_log.message = match[0];
	}
}
