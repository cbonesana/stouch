#include "RFIDMessage.h"

RFIDMessage::RFIDMessage(unsigned short id = 0x00)
{
    version = 0x8001;
    messageId = id;
    vendor = 0x00005358;
    length = 10;            // 10 header
}

RFIDMessage::RFIDMessage(unsigned char *buffer, unsigned int messageLength) throw()
{
    // cut the header
    version = bufferToShort(&buffer[HEAD_START]);
    messageId = bufferToShort(&buffer[MESSAGE_ID]);
    vendor = bufferToInt(&buffer[VENDOR_ID]);
    length = bufferToShort(&buffer[MSG_LENGTH]);

    unsigned int position = 10;

    // process the bodies
    MessageRFIDBody temp;

    do {
        temp.reserved = bufferToShort(&buffer[position]);
        temp.length = bufferToShort(&buffer[position +2]);
        temp.attributeType = bufferToShort(&buffer[position +4]);
        temp.attributeValue = new unsigned char[(temp.length -6)]();
        memcpy(temp.attributeValue, &buffer[position +6],(temp.length -6));
        position += temp.length;

        body.push_back(temp);
    } while (position < messageLength);
}

RFIDMessage::~RFIDMessage()
{}

int RFIDMessage::addCommand(unsigned short type, unsigned short value)
{
    unsigned char *temp = new unsigned char[2]();
    shortToBuffer(value,temp);
    return addCommand(type,temp,2);
}

int RFIDMessage::addCommand(unsigned short type, unsigned int value)
{
    unsigned char *temp = new unsigned char[4]();
    intToBuffer(value,temp);
    return addCommand(type,temp,4);
}

int RFIDMessage::addCommand(unsigned short type, unsigned long value, unsigned short len)
{
    unsigned char *temp = new unsigned char[len];
    longToBuffer(value,temp,len);
    return addCommand(type,temp,len);
}

int RFIDMessage::addCommand(unsigned short type, unsigned char *value, unsigned short len)
{
    if (len == 0)
        return -1;

    MessageRFIDBody element;

    element.reserved = 0;
    element.attributeType = type;
    element.attributeValue = value;
    element.length = 6 + len;

    length += element.length;

    body.push_back(element);

    return 0;
}

unsigned char *RFIDMessage::getBuffer()
{
    unsigned char *buffer = new unsigned char[length]();
    unsigned int index = 10;

    /* header */
    shortToBuffer(version, &buffer[0]);
    shortToBuffer(messageId, &buffer[2]);
    intToBuffer(vendor, &buffer[4]);
    shortToBuffer(length, &buffer[8]);

    /* body */
    for(std::vector<MessageRFIDBody>::iterator it = body.begin(); it != body.end(); ++it) {
        MessageRFIDBody temp = *it;

        shortToBuffer(temp.reserved, &buffer[index]);
        index += 2;
        shortToBuffer(temp.length, &buffer[index]);
        index += 2;
        shortToBuffer(temp.attributeType, &buffer[index]);
        index += 2;

        for (int i=0; i<(temp.length-6); i++){
            buffer[index] = temp.attributeValue[i];
            index++;
        }
    }

    return buffer;
}

unsigned short RFIDMessage::getLength()
{
    return length;
}

vector<RFID> *RFIDMessage::getRFIDs()
{
    vector<RFID> *founds = new vector<RFID>();
    RFID rfid;

    for(vector<MessageRFIDBody>::iterator it = body.begin(); it != body.end(); ++it){
        MessageRFIDBody *temp = &(*it);
        if (temp->attributeType == RFIDAttributeTypes::TAG_ID_LEN){
            rfid.length = bufferToShort(temp->attributeValue);
        }
        if (temp->attributeType == RFIDAttributeTypes::TAG_ID){
            rfid.rfid = temp->attributeValue;
            founds->push_back(rfid);
        }
    }

    return founds;
}

bool RFIDMessage::success()
{
    MessageRFIDBody last = (*body.end());
    if (last.attributeType == RFIDAttributeTypes::RESULT_CODE && last.attributeValue == RFIDCommandsCodes::SUCCESS){
        return true;
    }
    return false;
}

void RFIDMessage::print()
{
    printf("HEADER\n");
    printf("Version    : %04x\n", version);
    printf("Message ID : %04x\n", messageId);
    printf("Vendor     : %08x\n", vendor);
    printf("Length     : %04x\n", length);
    printf("\n");

    for(vector<MessageRFIDBody>::iterator it = body.begin(); it != body.end(); ++it){
        printf("Reserved   : %04x\n", (*it).reserved);
        printf("Length     : %04x\n", (*it).length);
        printf("Att. Type  : %04x\n", (*it).attributeType);

        printf("Att. Value : ");

        for (int i=0; i<((*it).length -6); i++){
            printf("%02x ", (*it).attributeValue[i]);
            fflush(stdout);
        }

        printf("\n\n");
    }
}

void RFIDMessage::shortToBuffer(unsigned short s, unsigned char *converted)
{
    converted[0] = (0xFF00 & s) >> 8;
    converted[1] = (0x00FF & s);
}

void RFIDMessage::intToBuffer(unsigned int i, unsigned char *converted)
{
    unsigned int mask = 0x000000FF;
    for (int j=0; j<4; j++){
        converted[3-j] = mask & i;
        i >>= 8;
    }
}

void RFIDMessage::longToBuffer(unsigned long l, unsigned char *converted, unsigned short len)
{
    unsigned long mask = 0xFF;
    for (int i=0; i<len; i++){
        converted[len-i] = mask & l;
        l >>= 8;
    }
}

unsigned short RFIDMessage::bufferToShort(unsigned char *buffer)
{
    return (buffer[0] << 8) + buffer[1];
}

unsigned int RFIDMessage::bufferToInt(unsigned char *buffer)
{
    unsigned int j=0;
    for (int i=0; i<4; i++){
        j <<= 8;
        j += buffer[i];
    }
    return j;
}

