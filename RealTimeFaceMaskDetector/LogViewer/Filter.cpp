#include "Filter.h"

logger::Filter::Filter(const std::vector<logger::LogOptions>& options_, const std::string& from_, \
	const std::string& till_) :
	options{ options_ }, from{ from_ }, till{ till_ }
{}

void logger::Filter::operator=(const Filter& other) {
	options = other.options;
	from = other.from;
	till = other.till;
}

bool logger::Filter::option_filter(const logger::LogOptions current_option) const {
	for (auto option : options) {
		if (current_option == option) {
			return true;
		}
	}
	return false;
}

bool logger::Filter::from_datetime_filter(DateTime& datetime) const {
	return from.empty() ? true : datetime >= from;
}

bool logger::Filter::till_datetime_filter(DateTime& datetime) const {
	bool res = till.empty() ? true : datetime <= till;
	return res;
}

void logger::Filter::set_options(const std::vector<LogOptions>& options_) {
	options = options_;
}

void logger::Filter::set_from_datetime(const std::string& datetime) {
	from = datetime;
}

void logger::Filter::set_till_datetime(const std::string& datetime) {
	till = datetime;
}

size_t logger::Filter::options_size() {
	return options.size();
}