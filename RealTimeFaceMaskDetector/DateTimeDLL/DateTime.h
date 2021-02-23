#ifndef DATETIMEDLL_DATETIME_H
#define DATETIMEDLL_DATETIME_H

#include "pch.h"
#include <string>
#include <regex>
#include "DateTimeException.h"

/*
* DateTime type which supports comparison of two datetimes objects. 
* Format of datatime should be "yyyy-mm-dd hh:mm:ss"
*/

class DateTime {
public:
	DateTime(const std::string& datetime_) : datetime{ datetime_ } {
		parse();
	}
	bool operator<(const DateTime&);
	bool operator>(const DateTime&);
	bool operator<=(const DateTime&);
	bool operator>=(const DateTime&);
	bool operator==(const DateTime&);
	bool operator!=(const DateTime&);

	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minutes;
	unsigned int seconds;
private:
	// Parse string to the int parts
	void parse();
	void parse_month();
	void parse_day();
	void parse_hour();
	void parse_minutes();
	void parse_seconds();
	void check_pattern();

	std::string datetime;
	std::regex pat{ R"(\d{4}-\d{2}-\d{2}\s{1}\d{2}:\d{2}:\d{2})" }; // Pattern to check that format is right
};

#endif // !DATETIMEDLL_DATETIME_H
