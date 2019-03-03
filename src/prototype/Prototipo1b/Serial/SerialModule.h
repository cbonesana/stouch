#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QString>
#include <QStringBuilder>
#include <QVector>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

#include "../Module.h"

#define MAX_BUFFER 4096

using namespace std;

class SerialModule : public Module
{
    Q_OBJECT

public:
    explicit SerialModule(QString config, QString device, QObject *parent = 0);
    explicit SerialModule(QString config, QString device, int speed, int parity, QObject *parent = 0);
    ~SerialModule();

    bool init(int speed, int parity);

    int send(unsigned char *inBuffer, unsigned int length);
    int receive(unsigned char *outBuffer, unsigned int length);

    unsigned char *sendAndReceive(unsigned char *sendBuffer, unsigned int sendLength, unsigned int *receivedLength);

signals:
    void dataReceived();
    void charReceived(char data);
    void finished();

    void log(QString sender, QString message, QString priority);
    void serialerror(QString err);

public slots:
    void process();
    void stop();

private:
    QString        deviceName;

    int            device;
    struct termios tty;
    bool           working;

    int setInterfaceAttribs(int fd, int speed, int parity);
    int setBlocking(int fd, bool shouldBlock);
};

#endif // SERIALTHREAD_H
