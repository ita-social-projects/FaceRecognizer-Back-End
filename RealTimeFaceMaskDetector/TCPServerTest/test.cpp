#include "pch.h"
#include "../TCPServer/WindowsService.cpp"

TEST(TestWindowsService, TestServiceName) 
{
	std::wstring test_name = L"Test_Name";
	Service::set_service_name(test_name);
	ASSERT_EQ("Test_Name", Service::get_service_name());

}