#include "Exception.hpp"

Exception::Exception(std::string err_msg)
{
    error_msg = err_msg;
}

std::string Exception::what()
{
    return error_msg;
}