#include "SignalsException.h"

SignalsException::SignalsException()
{
    message = "Button Exception.";
}

SignalsException::SignalsException(string error)
{
    message = "Button Exception: " + error;
}

SignalsException::~SignalsException() throw()
{}

const char *SignalsException::what() const throw()
{
    return message.c_str();
}
