#ifndef LOGVIEWER_STRUCTURES_H
#define LOGVIEWER_STRUCTURES_H
#include "../DateTimeDLL/DateTime.h"

namespace logger {
	enum LogOptions {
		INFO,
		WARNING,
		ERR,
		DEBUG,
		FATAL,
		TRACE
	};

	struct LogStruct {
		DateTime datetime;
		LogOptions option;
		std::string trace;
		std::string message;
	};
}

#endif // !LOGVIEWER_STRUCTURES_H
