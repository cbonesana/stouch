#ifndef RFIDMODULE_H
#define RFIDMODULE_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringBuilder>

#include "RFIDMessage.h"
#include "../Serial/SerialModule.h"
#include "../Module.h"

using namespace std;

class RFIDModule : public Module
{
    Q_OBJECT

public:
    RFIDModule(QString config, SerialModule *module, QObject *parent=0);

    static char *parseRFID(RFID rfid);

    RFIDMessage *sendAndRecieve(RFIDMessage *message);

    RFIDMessage *getAntennaStatus(unsigned char *source, unsigned short len, unsigned short id=0x0000);
    RFIDMessage *getPower(unsigned short id=0x0000);
    RFIDMessage *setPower(unsigned int powerLevel, unsigned short id=0x0000);
    RFIDMessage *setProtocol(unsigned int protocol, unsigned short id=0x0000);
    RFIDMessage *getProtocol(unsigned short id=0x0000);
    RFIDMessage *inventory(unsigned char *source, unsigned short len, unsigned short id=0x0000);

signals:
    void log(QString sender, QString message, QString priority = "LOG");

private:
    SerialModule *serialModule;
};

#endif // RFIDMODULE_H
