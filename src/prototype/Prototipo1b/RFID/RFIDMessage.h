#ifndef RFIDMESSAGE_H
#define RFIDMESSAGE_H

#include "RFIDattributeTypes.h"
#include "RFIDCommandsCodes.h"
#include "RFIDException.h"

#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdio>

#define HEAD_START 0
#define MESSAGE_ID 2
#define VENDOR_ID 4
#define MSG_LENGTH 8

typedef struct MessageRFIDBody {
    unsigned short reserved;        // 2
    unsigned short length;          // 2
    unsigned short attributeType;   // 2
    unsigned char *attributeValue;  // *

} MessageRFIDBody;

typedef struct RFID {
    unsigned char *rfid;
    unsigned short length;
} RFID;

using namespace std;

class RFIDMessage
{
public:
    RFIDMessage(unsigned short id);
    RFIDMessage(unsigned char *buffer, unsigned int messageLength) throw();
    virtual ~RFIDMessage();

    int addCommand(unsigned short type, unsigned short value);
    int addCommand(unsigned short type, unsigned int value);
    int addCommand(unsigned short type, unsigned long value, unsigned short len);
    int addCommand(unsigned short type, unsigned char *value, unsigned short len);

    unsigned char *getBuffer();
    unsigned short getLength();

    vector<RFID> *getRFIDs();
    bool success();

    void print();

private:
    /* header */
    unsigned short version;     // 2
    unsigned short messageId;   // 2
    unsigned int   vendor;      // 4
    unsigned short length;      // 2

    std::vector<MessageRFIDBody> body;

    void shortToBuffer(unsigned short s, unsigned char *converted);
    void intToBuffer(unsigned int i, unsigned char *converted);
    void longToBuffer(unsigned long l, unsigned char *converted, unsigned short len);

    unsigned short bufferToShort(unsigned char *buffer);
    unsigned int bufferToInt(unsigned char *buffer);
};

#endif // RFIDMESSAGE_H
