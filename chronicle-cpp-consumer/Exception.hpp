#pragma once
#include <string>

class Exception
{
    public:
        Exception(std::string err_msg);
        std::string what();

    private:
        std::string error_msg;
};