#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include <QStringBuilder>
#include <QRegExp>

#include <iostream>

#include "HTTPResponse.h"

using namespace std;

/*!
 * \class HTTPManager
 * \brief The HTTPManager class is used to contact the server, but not to send files.
 *
 * The HTTPManager class manages all the connections with the server. It can call servlets on the server to che ids and parameters.
 * This class do NOT send files: it is only used to make and HTTP request and manage the answears from the server.
 * Usually, the server respond with a JSON string of data.
 */
class HTTPManager : public QObject
{
    Q_OBJECT
public:
    HTTPManager(QObject *parent = 0);
    ~HTTPManager();

    QNetworkReply *contactServer(QString host);
    QNetworkReply *contactServer(QString host, QString idUser, QString command, QString mac, QString param, QString risposta);
    
    QString getMacAddress();
    QString getIpAddress();

signals:
    void responseAvailable(QString item, HTTPResponse *reply);
    void connectionError(QString error);
    
public slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;

};

#endif // HTTPMANAGER_H
