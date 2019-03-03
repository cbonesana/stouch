#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

class SerialThread : public QThread
{
    Q_OBJECT

public:
    explicit SerialThread(const char *device, QObject *parent = 0);
    
    void start();

signals:
    void dataReceived();
    void charReceived(char data);

public slots:

private:
    const char *deviceName;

    int device;
    struct termios tty;

    void run();
    int setInterfaceAttribs(int fd, int speed, int parity);
    int setBlocking(int fd, bool shouldBlock);
};

#endif // SERIALTHREAD_H
