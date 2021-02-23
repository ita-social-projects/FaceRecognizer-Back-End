#include "pch.h"
#include "../DateTimeDLL/DateTime.cpp"
#include "../DateTimeDLL/DateTimeException.cpp"

void check_exception(const std::string&);

TEST(DateTimeTests, CreateDateTime) {
	const std::string datetime_str{ "2021-02-11 16:44:19" };
	const unsigned int year{ 2021 };
	const unsigned int month{ 02 };
	const unsigned int day{ 11 };
	const unsigned int hour{ 16 };
	const unsigned int minutes{ 44 };
	const unsigned int seconds{ 19 };

	auto datetime = DateTime(datetime_str);

	EXPECT_EQ(datetime.year, year);
	EXPECT_EQ(datetime.month, month);
	EXPECT_EQ(datetime.day, day);
	EXPECT_EQ(datetime.hour, hour);
	EXPECT_EQ(datetime.minutes, minutes);
	EXPECT_EQ(datetime.seconds, seconds);
}

TEST(DateTimeTests, MonthException) {
	const std::string datetime_str{ "2021-14-11 16:44:19" };
	check_exception(datetime_str);
}

TEST(DateTimeTests, DayException) {
	const std::string datetime_str{ "2021-11-42 16:44:19" };
	check_exception(datetime_str);
}

TEST(DateTimeTests, HourException) {
	const std::string datetime_str{ "2021-11-30 27:44:19" };
	check_exception(datetime_str);
}

TEST(DateTimeTests, MinutesException) {
	const std::string datetime_str{ "2021-11-30 21:65:19" };
	check_exception(datetime_str);
}

TEST(DateTimeTests, SecondsException) {
	const std::string datetime_str{ "2021-11-30 21:43:78" };
	check_exception(datetime_str);
}

TEST(DateTimeTests, InvalidPatternThrowsException) {
	const std::string datetime_str{ "2021-11-3 21:43:78" };
	check_exception(datetime_str);
}

void check_exception(const std::string& datetime_str) {
	try {
		auto datetime = DateTime(datetime_str);
		FAIL();
	}
	catch (DateTimeException) {
		SUCCEED();
	}
}

TEST(DateTimeTests, LessThanTrue) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2019-10-29 20:41:44" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);
	
	EXPECT_TRUE(datetime2 < datetime1);
}

TEST(DateTimeTests, LessThanFalse) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };
	
	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_FALSE(datetime2 < datetime1);
}

TEST(DateTimeTests, MoreThanTrue) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2019-10-29 20:41:44" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_TRUE(datetime1 > datetime2);
}

TEST(DateTimeTests, MoreThanFalse) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_FALSE(datetime1 > datetime2);
}

TEST(DateTimeTests, LessThanEqualTrue) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_TRUE(datetime2 <= datetime1);
}

TEST(DateTimeTests, LessThanEqualFalse) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:46" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_FALSE(datetime2 <= datetime1);
}

TEST(DateTimeTests, MoreEqualThanTrue) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_TRUE(datetime2 >= datetime1);
}

TEST(DateTimeTests, MoreEqualThanFalse) {
	const std::string datetime1_str{ "2021-11-30 21:43:46" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_FALSE(datetime2 >= datetime1);
}

TEST(DateTimeTests, EqualTrue) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_TRUE(datetime2 == datetime1);
}

TEST(DateTimeTests, EqualFalse) {
	const std::string datetime1_str{ "2021-11-30 21:43:46" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_FALSE(datetime2 == datetime1);
}

TEST(DateTimeTests, NotEqualTrue) {
	const std::string datetime1_str{ "2021-11-30 21:42:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_TRUE(datetime2 != datetime1);
}

TEST(DateTimeTests, NotEqualFalse) {
	const std::string datetime1_str{ "2021-11-30 21:43:45" };
	const std::string datetime2_str{ "2021-11-30 21:43:45" };

	auto datetime1 = DateTime(datetime1_str);
	auto datetime2 = DateTime(datetime2_str);

	EXPECT_FALSE(datetime2 != datetime1);
}
