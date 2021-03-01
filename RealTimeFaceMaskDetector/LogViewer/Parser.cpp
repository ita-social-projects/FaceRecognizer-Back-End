#include "Parser.h"
#include <iostream>

void logger::Parser::parse_all() {
	while (getline(fs, line)) {
		logs.push_back(current_log);
	}
}

void logger::Parser::parse_with_options() {
	while (getline(fs, line)) {
		find_type();
		if (match) {
			logs.push_back(current_log);
			find_trace();
			find_message();
			match = false;
		}
	}
}

void logger::Parser::find_type() {
	if (std::regex_search(line, info_pat)) {
		current_log.option = LogOptions::INFO;
	}
	else if (std::regex_search(line, warning_pat)) {
		current_log.option = LogOptions::WARNING;
	}
	else if (std::regex_search(line, error_pat)) {
		current_log.option = LogOptions::ERR;
	}
	else if (std::regex_search(line, debug_pat)) {
		current_log.option = LogOptions::DEBUG;
	}
	else if (std::regex_search(line, fatal_pat)) {
		current_log.option = LogOptions::FATAL;
	}
}

void logger::Parser::parse_from_time() {
	while (getline(fs, line)) {
		find_time();
		if (current_log.time >= from) {
			logs.push_back(current_log);
			find_trace();
			find_message();
		}
	}
}

void logger::Parser::parse_from_till_time() {
	while (getline(fs, line)) {
		find_time();

		if (current_log.time >= from && current_log.time <= till) {
			logs.push_back(current_log);
			find_trace();
			find_message();
		}
	}
}

void logger::Parser::find_time() {
	std::smatch matches;
	if (!std::regex_search(line, matches, from.regex_pat())) {
		// TODO: throw exception
	}

	current_log.time = matches[0].str();
}

void logger::Parser::open_file() {
	fs.open(path, std::ios_base::in);

	if (!fs) {
		// TODO: Exception
		std::cerr << "Could not open a file";
		return;
	}
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
