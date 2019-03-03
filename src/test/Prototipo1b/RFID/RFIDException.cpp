#include "RFIDException.h"

RFIDException::RFIDException() {
    message = "RFID Message Exception.";
}

RFIDException::RFIDException(string error) {
    message = "RFID Message Exception: " + error + ".";
}

RFIDException::~RFIDException() throw() {

}

const char *RFIDException::what() const throw() {
    return message.c_str();
}
