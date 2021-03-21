#include "pch.h"
#include <thread>
#include "net_message.h"
#include "net_tsqueue.h"

enum class MessageTypes : uint32_t {
	Image,
	Text,
	JSON
};

void add_message_data(net::message<MessageTypes>&);

TEST(NetFrameworksTests, MessageSizeTest) {
	net::message<MessageTypes> msg;
	msg.header.id = MessageTypes::Image;

	EXPECT_EQ(msg.size(), 0);
}

TEST(NetFrameworkTests, WritePODDataIntoMessage) {
	net::message<MessageTypes> msg;

	add_message_data(msg);

	EXPECT_EQ(msg.size(), 49);
}

TEST(NetFrameworkTests, ReadPODDataFromMessage) {
	net::message<MessageTypes> msg;

	add_message_data(msg);
	int a;
	bool b;
	float c;

	struct
	{
		float x;
		float y;
	} d[5];
	msg >> d >> c >> b >> a;

	EXPECT_EQ(a, 1);
	EXPECT_TRUE(b);
	EXPECT_EQ(c, 3.14159f);
	EXPECT_EQ(d[0].x, 21.4f);
	EXPECT_EQ(d[0].y, 22.5f);
}

void add_message_data(net::message<MessageTypes>& msg) {
	int a = 1;
	bool b = true;
	float c = 3.14159f;

	struct
	{
		float x;
		float y;
	} d[5];

	d[0].x = 21.4f;
	d[0].y = 22.5f;
	msg << a << b << c << d;
}

TEST(NetFrameworkTest, TestPushQueue) {
	net::tsqueue<int> queue;
	std::thread th1{ [&] { queue.push(2); } };
	std::thread th2{ [&] { queue.push(2); } };

	th1.join();
	th2.join();
	EXPECT_EQ(queue.count(), 2);
}

TEST(NetFrameworkTest, TestClearQueue) {
	net::tsqueue<int> queue;
	
	queue.push(1);
	queue.push(2);

	queue.clear();

	EXPECT_TRUE(queue.empty());
}