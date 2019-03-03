#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include "FileParser.h"

class Module : public QObject
{
    Q_OBJECT

public:
    explicit Module(QString config, QString componentName, QObject *parent = 0);
    virtual ~Module();

    QString componentName;

signals:
    void log(QString sender, QString message, QString priority = "LOG");
    
protected:
    FileParser *fileParser;
};

#endif // MODULE_H
