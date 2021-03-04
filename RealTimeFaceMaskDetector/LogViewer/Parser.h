#ifndef LOGVIEWER_PARSER_H
#define LOGVIEWER_PARSER_H
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include "Filter.h"

namespace logger {
	// Class for parsing a log file. It divides logs on datetime parts, message part, trace part and option part.
	class Parser {
	public:
		Parser(const std::string&);

		/* 
		* Method for parsing a file with passed paramaters
		* options - it's a sequence of options which should be added to the result
		* from - it's a time from which log shoud be added to the result. If there is no need in this parameter - pass "0000-00-00 00:00:00"
		* till - it's a time till log should be added to the result. If there is no need in this parameter - pass "0000-00-00 00:00:00"
		*/
		void parse(const std::vector<LogOptions>&, const std::string&, const std::string&);

		~Parser() {
			fs.close();
		}

		const std::vector<LogStruct>& get_logs() const { return logs; }
	private:
		void open_file(const std::string&);

		bool parse_option();
		bool parse_time();

		void find_type();
		void find_time();
		void find_trace();
		void find_message();

		void clear();
		const int number_of_options{ 6 };

		std::vector<LogStruct> logs;
		std::fstream fs;
		std::string line;

		std::regex option_pat{ R"(\[\w*\])" };
		std::regex trace_pat{ R"(-\s\[.*\])" };
		std::regex message_pat{ R"(\w(\w|\s)*$)" };
		std::smatch matches;
		
		LogStruct current_log;
		Filter filter;
	};

}

#endif //LOGVIEWVER_PARSER_H