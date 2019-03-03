#ifndef RFIDEXCEPTION_H
#define RFIDEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class RFIDException : public exception
{
public:
    RFIDException();
    RFIDException(string error);

    virtual ~RFIDException() throw();
    virtual const char *what() const throw();

private:
    string message;
};

#endif // RFIDEXCEPTION_H
