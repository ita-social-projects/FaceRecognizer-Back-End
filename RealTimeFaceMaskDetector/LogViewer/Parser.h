#ifndef LOGVIEWER_PARSER_H
#define LOGVIEWER_PARSER_H
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include "../DateTimeDLL/DateTime.h"

namespace logger {
	enum class LogOptions {
		INFO,
		WARNING,
		ERR,
		DEBUG,
		FATAL
	};

	struct LogStruct {
		DateTime time;
		LogOptions option;
		std::string trace;
		std::string message;
	};

	class Parser {
	public:
		Parser(const std::string& path_) : path{ path_ } {
			open_file();
			parse_all();
		}

		Parser(const std::string& path_, const std::vector<LogOptions>& options_) : 
			options{ options_ }, path{path_} 
		{
			open_file();
			parse_with_options();
		}

		Parser( const std::string& path_, const std::string& from_) :
			path{ path_ }, from{ from_ }
		{
			open_file();
			parse_from_time();
		}

		Parser ( const std::string& path_, const std::string& from_, const std::string& till_) :
			path{ path_ }, from{ from_ }, till{ till_ } 
		{
			open_file();
			parse_from_till_time();
		}
		
		~Parser() {
			fs.close();
		}

		const std::vector<LogStruct>& get_logs() const { return logs; }
	private:
		void open_file();
		void parse_all();
		void parse_with_options();
		void find_type();
		void parse_from_time();
		void parse_from_till_time();
		void find_time();
		void find_trace();
		void find_message();

		bool match{ false };
		std::string path;
		std::vector<LogOptions> options;
		std::vector<LogStruct> logs;
		std::fstream fs;
		std::string line;

		std::regex info_pat{R"(\[INFO\]{1})"};
		std::regex warning_pat{ R"(\[WARNING\]{1})" };
		std::regex error_pat{ R"(\[ERROR\]{1})" };
		std::regex debug_pat{ R"(\[DEBUG\]{1})" };
		std::regex fatal_pat{ R"(\[FATAL\]{1})" };
		std::regex trace_pat{ R"(-\s\[.*\])" };
		std::regex message_pat{ R"(\w(\w|\s)*$)" };
		
		LogStruct current_log;
		DateTime from;
		DateTime till;
	};

}

#endif //LOGVIEWVER_PARSER_H