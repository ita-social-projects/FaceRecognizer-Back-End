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
			match = false;
		}
	}
}

void logger::Parser::find_type() {
	for (auto option : options) {
		if (option == LogOptions::INFO && std::regex_search(line, info_pat)) {
			current_log.option = LogOptions::INFO;
			match = true;
		}
		else if (option == LogOptions::WARNING && std::regex_search(line, warning_pat)) {
			current_log.option = LogOptions::WARNING;
			match = true;
		}
		else if (option == LogOptions::ERR && std::regex_search(line, error_pat)) {
			current_log.option = LogOptions::ERR;
			match = true;
		}
		else if (option == LogOptions::DEBUG && std::regex_search(line, debug_pat)) {
			current_log.option = LogOptions::DEBUG;
			match = true;
		}
		else if (option == LogOptions::FATAL && std::regex_search(line, fatal_pat)) {
			current_log.option = LogOptions::FATAL;
			match = true;
		}
	}
}

void logger::Parser::parse_with_time() {
	while (getline(fs, line)) {
		find_time();
	}
}

void logger::Parser::find_time() {
	std::smatch matches;
	//if (!std::regex_match(line, matches, DateTime::datetime_pat)) {
	//	// TODO: throw exception
	//}

	DateTime datetime_log{ matches[1] };
	/*if (datetime >= datetime_log) {
		match = true;
	}
	else {
		match = false;
	}*/
}

void logger::Parser::open_file() {
	fs.open(path, std::ios_base::in);

	if (!fs) {
		// TODO: Exception
		std::cerr << "Could not open a file";
		return;
	}
}
