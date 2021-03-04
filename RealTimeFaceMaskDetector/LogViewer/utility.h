#ifndef LOGVIEWER_UTILITY_H
#define LOGVIEWER_UTILITY_H

#include <sstream>
#include <iomanip>
#include <map>
#include <QColor>
#include "Parser.h"
#include "structures.h"

using namespace logger;

static const std::map<LogOptions, const char*> log_opt_to_str{
		{ LogOptions::INFO, "INFO" },
		{ LogOptions::WARNING, "WARN" },
		{ LogOptions::ERR, "ERROR" },
		{ LogOptions::DEBUG, "DEBUG" },
		{ LogOptions::FATAL, "FATAL" },
		{ LogOptions::TRACE, "TRACE" },
};

static const std::map<LogOptions, QColor> log_opt_to_color{
	{ LogOptions::INFO, QColor{ 0, 191, 255, 114 } },
	{ LogOptions::WARNING, QColor{ 255, 215, 0, 114 } },
	{ LogOptions::ERR, QColor{ 255, 99, 71, 215 } },
	{ LogOptions::DEBUG, QColor{ 169, 169, 169, 114 } },
	{ LogOptions::FATAL, QColor{ 178, 34, 34, 225 } },
	{ LogOptions::TRACE, QColor{ 144, 238, 144, 230 } },
};

using VectorLogStruct = std::vector<LogStruct>;
static VectorLogStruct ParseLogFile(
	const std::string& path,
	const std::vector<LogOptions>& log_level_options,
	const std::string& since,
	const std::string& till
) {
	Parser parser(path);
	parser.parse(log_level_options, since, till);
	return parser.get_logs();
}

static QTableWidgetItem* SetBackgroundColor(QTableWidgetItem* twi, const QColor& color)
{
	twi->setBackgroundColor(color);
	if (color == log_opt_to_color.at(LogOptions::FATAL))
	{
		twi->setForeground(QColor{ 250, 250, 250, 255 });
	}
	return twi;
}

#endif // LOGVIEWER_UTILITY_H