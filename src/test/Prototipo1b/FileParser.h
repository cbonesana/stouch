#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class FileParser
{
public:
    FileParser(QString filename);
    ~FileParser();

    QString parseFor(QString name);
    QString parseFor(QString name, QString alt);

private:
    QString filename;
    QFile *configFile;
};

#endif // FILEPARSER_H
