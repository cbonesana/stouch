#ifndef IOMODULE_H
#define IOMODULE_H

#include <QObject>

#include <stdio.h>
#include <iostream>

class IOModule : public QObject
{
    Q_OBJECT
public:
    explicit IOModule(QObject *parent = 0);
    
signals:
    void logData(QString sender, QString message, QString priority);
    void signal();
    void signalString(QString str);
    void finished();

public slots:
    void handleError(QString err);
    void handleEvent();

    void process();
    
};

#endif // IOMODULE_H
