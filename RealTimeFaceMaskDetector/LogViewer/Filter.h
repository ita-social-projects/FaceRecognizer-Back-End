#ifndef LOGVIEWER_FILTER_H
#define LOGVIEWER_FILTER_H
#include <string>
#include "structures.h"

namespace logger {
	class Filter {
	public:
		Filter(const std::vector<LogOptions>&, const std::string&, const std::string&);
		Filter() = default;

		void operator=(const Filter&);

		bool option_filter(const LogOptions) const;
		bool from_datetime_filter(DateTime&) const;
		bool till_datetime_filter(DateTime&) const;

		void set_options(const std::vector<LogOptions>&);
		void set_from_datetime(const std::string&);
		void set_till_datetime(const std::string&);

		size_t options_size();
	private:
		std::vector<LogOptions> options;
		DateTime from;
		DateTime till;
	};
}
#endif // !LOGVIEWER_FILTER_H

