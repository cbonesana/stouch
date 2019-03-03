#ifndef SIGNALPOOL_H
#define SIGNALPOOL_H

#include <QObject>

#include <stdio.h>

#include "serialthread.h"

class SignalPool : public QObject
{
    Q_OBJECT
public:
    explicit SignalPool(SerialThread *serialThread, QObject *parent = 0);
    
signals:
    
public slots:
    void newData();
    void newChar(char c);
};

#endif // SIGNALPOOL_H
