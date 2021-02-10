#include "pch.h"
#include "SQLException.h"

const char* SQLException::what() const throw()
{
	return err_message.c_str();
}

int SQLException::GetErrCode()
{
	return err_code;
}

std::string SQLException::GetErrMessage()
{
	return err_message;
}