#pragma once
#include <exception> 
#include <string>
/*Exception class for SQLConnection*/
class SQLException : public std::exception {
public:
    /*Overriden function from the base class*/
    const char* what() const throw();
    /*Get inc code of the error*/
    int GetErrCode();
    /*Get string message for the error*/
    std::string GetErrMessage();
protected:
    std::string err_message;
    int err_code;
};