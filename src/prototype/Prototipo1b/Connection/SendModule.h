#ifndef SENDMODULE_H
#define SENDMODULE_H

#include <QObject>
#include <QQueue>
#include <QFile>
#include <QString>
#include <QByteArray>

#include <cerrno>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netdb.h>

#include "SocketModule.h"
#include "ConnectionImage.h"
#include "../Module.h"
#include "../config.h"

using namespace std;

/*!
 * \class SendModule
 * \brief The SendModule class is used to send files to the server.
 *
 * The SendModule class opens a socket to the server on a specified port and send a file encoded in base64.
 * The files are taken from a queue, encoded and sent to a server only if in online mode.
 *
 * Someone have to signal to this class to start working on the queue with a poke signal. When it starts
 * working on the queue, no more pokes are managed until the queue is empty. The processing of the queue
 * works only in online mode.
 */
class SendModule : public Module
{
    Q_OBJECT

public:
    SendModule(QString config, QQueue<ConnectionImage> *queue);
    ~SendModule();

signals:
    void taskStarted();
    void taskCompleted();

    void internalTaskStart();
    void contactHttpServer(QString host, QString idUser, QString comando, QString mac, QString param, QString risposta);
    void externalCropImage(QString data, QString filename, QString userId);

    void socketSendFinished();
    void connectionErrorSender(QString error, int nerr);
    void readImageFileError();

public slots:
    void startTask();

    void goOfflineMode();
    void goOnlineMode();

    void manageSocketError(QString error, int nerr);
    void manageSocketFinished();
    void manageTestRFID(QString id, QString error);
    void manageTestCrop(QString data);

private:
    QQueue<ConnectionImage> *queueData;

    SocketModule *socketModule;

    bool wirelessAvailable;
    bool onlineMode;

    bool isConnectionAvailable();
};

#endif // SENDMODULE_H
