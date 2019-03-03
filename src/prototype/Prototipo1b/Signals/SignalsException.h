#ifndef BUTTONEXCEPTION_H
#define BUTTONEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class SignalsException : public exception
{
public:
    SignalsException();
    SignalsException(string error);

    virtual ~SignalsException() throw();
    virtual const char* what() const throw();

private:
    string message;
};

#endif // BUTTONEXCEPTION_H
