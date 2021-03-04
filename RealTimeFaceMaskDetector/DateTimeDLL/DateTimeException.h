#ifndef DATETIMEDLL_DATETIMEEXCEPTION_H
#define DATETIMEDLL_DATETIMEEXCEPTION_H

#include "pch.h"
#include <string>
#include <exception>

class DateTimeException : public std::exception {
public:
	DateTimeException(const std::string& message_) : message{ message_ } {}
	const char* what() const throw() {
		return message.c_str();
	};
private:
	std::string message;
};

#endif // DATETIMEDLL_DATETIMEEXCEPTION_H
