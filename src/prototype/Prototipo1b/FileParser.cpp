#include "FileParser.h"

FileParser::FileParser(QString filename) : filename(filename)
{
    configFile = new QFile(filename);
}

FileParser::~FileParser()
{
    if (configFile->isOpen()){
        configFile->close();
    }
    delete(configFile);
}

QString FileParser::parseFor(QString name)
{
    bool status = configFile->open(QIODevice::ReadOnly);
    if (!status) {
        return "";
    }

    QString foundPath = QString("");

    QTextStream in(configFile);
    while(!in.atEnd()){
        QString line = in.readLine();

        if (line.startsWith(name)){
            QStringList list = line.split(":");
            foundPath = list.at(1).simplified();
        }
    }

    configFile->close();

    return foundPath;
}

QString FileParser::parseFor(QString name, QString alt)
{
    QString path = parseFor(name);
    if (path == ""){
        path = alt;
    }
    return path;
}

