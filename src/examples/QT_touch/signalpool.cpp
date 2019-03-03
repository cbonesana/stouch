#include "signalpool.h"

SignalPool::SignalPool(SerialThread *serialThread, QObject *parent) :
    QObject(parent)
{
    connect(serialThread, SIGNAL(dataReceived()), this, SLOT(newData()));
    connect(serialThread, SIGNAL(charReceived(char)), this, SLOT(newChar(char)));
}

void SignalPool::newData()
{
    printf("New Data!!!\n");
}

void SignalPool::newChar(char c)
{
    printf("%c", c);
    fflush(stdout);
}
