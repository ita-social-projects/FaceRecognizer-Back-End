#ifndef LOGVIEWER_LOGVIEWEREXCEPTION
#define LOGVIEWER_LOGVIEWEREXCEPTION
#include <string>
#include <exception>

namespace logger {
	class LogViewerException : public std::exception {
	public:
		LogViewerException(const std::string& message_) : message{ message_ } {}
		const char* what() const throw() {
			return message.c_str();
		};
	private:
		std::string message;
	};
}
#endif // !LOGVIEWER_LOGVIEWREXCEPTION
