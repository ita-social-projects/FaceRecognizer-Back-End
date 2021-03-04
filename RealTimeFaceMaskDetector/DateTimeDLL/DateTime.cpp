#include "pch.h"
#include <sstream>
#include <iomanip>
#include "DateTime.h"

void DateTime::parse() {
	check_pattern();
	year = std::stoi(datetime.substr(0, 4));
	parse_month();
	parse_day();
	parse_hour();
	parse_minutes();
	parse_seconds();
}

void DateTime::check_pattern() {
	if (!std::regex_search(datetime, datetime_pat)) {
		throw DateTimeException("A format of datetime is invalid");
	}
}

void DateTime::parse_month() {
	month = std::stoi(datetime.substr(5, 7));

	if (month > 12) {
		throw DateTimeException("Month is invalid");
	}
}

void DateTime::parse_day() {
	day = std::stoi(datetime.substr(8, 10));

	if (day > 31) {
		throw DateTimeException("Day is invalid");
	}
}

void DateTime::parse_hour() {
	hour = std::stoi(datetime.substr(11, 13));

	if (hour > 24) {
		throw DateTimeException("Hour is invalid");
	}
}

void DateTime::parse_minutes() {
	minutes = std::stoi(datetime.substr(14, 16));

	if (minutes > 60) {
		throw DateTimeException("Value of minutes is invalid");
	}
}

void DateTime::parse_seconds() {
	seconds = std::stoi(datetime.substr(17, 19));

	if (seconds > 60) {
		throw DateTimeException("Value of seconds is invalid");
	}
}

bool DateTime::operator<(const DateTime& datetime) {
	return year < datetime.year || year == datetime.year && \
			(month < datetime.month || month == datetime.month && \
				(day < datetime.day || day == datetime.day && \
					(hour < datetime.hour || hour == datetime.hour && \
						(minutes < datetime.minutes || minutes == datetime.minutes && \
							(seconds < datetime.seconds)
						)
					)
				)
			);
}

bool DateTime::operator>(const DateTime& datetime) {
	return year > datetime.year || year == datetime.year && \
			(month > datetime.month || month == datetime.month && \
				(day > datetime.day || day == datetime.day && \
					(hour > datetime.hour || hour == datetime.hour && \
						(minutes > datetime.minutes || minutes == datetime.minutes && \
							(seconds > datetime.seconds)
						)
					)
				)
			);
}

bool DateTime::operator<=(const DateTime& datetime) {
	return year < datetime.year || year == datetime.year && \
		(month < datetime.month || month == datetime.month && \
			(day < datetime.day || day == datetime.day && \
				(hour < datetime.hour || hour == datetime.hour && \
					(minutes < datetime.minutes || minutes == datetime.minutes && \
						(seconds <= datetime.seconds)
						)
					)
				)
			);
}

bool DateTime::operator>=(const DateTime& datetime) {
	return year > datetime.year || year == datetime.year && \
		(month > datetime.month || month == datetime.month && \
			(day > datetime.day || day == datetime.day && \
				(hour > datetime.hour || hour == datetime.hour && \
					(minutes > datetime.minutes || minutes == datetime.minutes && \
						(seconds >= datetime.seconds)
						)
					)
				)
			);
}

bool DateTime::operator==(const DateTime& datetime) {
	if (year != datetime.year) {
		return false;
	}
	else if (month != datetime.month) {
		return false;
	}
	else if (day != datetime.day) {
		return false;
	}
	else if (hour != datetime.hour) {
		return false;
	}
	else if (minutes != datetime.minutes) {
		return false;
	}
	else if (seconds != datetime.seconds) {
		return false;
	}
	return true;
}

bool DateTime::operator!=(const DateTime& datetime) {
	if (*this == datetime) {
		return false;
	}
	return true;
}

const std::regex& DateTime::regex_pat() const {
	return datetime_pat;
}

bool DateTime::empty() const {
	return (!year && !month && !day && !hour && !minutes && !seconds);
}

const std::string DateTime::str() const {
	std::ostringstream string_stream;
	string_stream << std::setfill('0') << std::setw(4) << year << "-" << std::setw(2) << month \
		<< "-" << std::setw(2) << day << " " << std::setw(2) << hour << ":" << std::setw(2) << minutes \
		<< ":" << std::setw(2) << seconds;
	return string_stream.str();
}
