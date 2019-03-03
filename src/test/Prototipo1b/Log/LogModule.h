#ifndef LOGGINGTHREAD_H
#define LOGGINGTHREAD_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QStringBuilder>
#include <QFile>
#include <QTextStream>

#include <QTime>
#include <QDate>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../Module.h"
#include "../config.h"

using namespace std;

class LogModule : public Module
{
    Q_OBJECT

public:
    LogModule(QString config, QObject *parent=0);
    ~LogModule();

public slots:
    void logging(QString sender, QString message, QString priority = "LOG  ");

signals:
    void finished();
    void error(QString err);
    void logMessage(QString message);

private:
    QString logFileName;
    QFile *logFile;

};

#endif // LOGGINGTHREAD_H
