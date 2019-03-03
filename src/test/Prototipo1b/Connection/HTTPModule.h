#ifndef HTTPMODULE_H
#define HTTPMODULE_H

#include <QObject>
#include <QDebug>

#include "HTTPManager.h"
#include "HTTPResponse.h"
#include "../Module.h"
#include "../config.h"


/*!
 * \class HTTPModule
 *
 * \brief The HTTPModule class is a module used to contact a remote server.
 *
 * The HTTPModule class is used to contact a remote server. Every request for the server, with the exception of files, arrives to this module.
 * Every reply from the server is managed, parsend and the result sent to the right module connected to this one.
 */
class HTTPModule : public Module
{
    Q_OBJECT

public:
    HTTPModule(QString config, bool debug = true);
    ~HTTPModule();

signals:

    void sigBackScan();
    void sigCheckUserDet(QString id, QString mac);
    void sigSetRFID(QString id, QString error);
    void sigTestRFID(QString id, QString error);
    void sigGetCrop(QString data);
    void sigTestCrop(QString data);

    void connectionErrorHttp(QString error);

public slots:

    void manageConnectionError(QString error);
    void manageResponses(QString item, HTTPResponse * response);

    void contactServer(QString host, QString idUser, QString comando, QString mac, QString param, QString risposta);

    void goOfflineMode();
    void goOnlineMode();

    void logOnServer(QString message);

private:

    HTTPManager *httpManager;

    QString deviceId;
    QString ipAddress;
    QString macAddress;

    bool debug;
    bool onlineMode;

    HTTPManager *getHttpManager();
};

#endif // HTTPMODULE_H
