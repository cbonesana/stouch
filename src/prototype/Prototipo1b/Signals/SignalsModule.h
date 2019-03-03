#ifndef BUTTONMODULE_H
#define BUTTONMODULE_H

#include <QObject>

#include <QString>
#include <QStringBuilder>

#include <fstream>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "SignalsException.h"
#include "../Log/LogModule.h"
#include "../Module.h"

#define D_REGIST_CLIENT 1   // register a new client
#define D_OFFLINE_MODE  2   // remove a client
#define D_ONLINE_MODE   3   // toggle button acquisition
#define D_ACKNOWLEDGE   4   // acknowledge
#define D_STOP_CLIENT   5   // stop client signal
#define D_ERROR         -1  // error


using namespace std;

class SignalsModule : public Module
{
    Q_OBJECT

public:
    SignalsModule(QString config, LogModule *logModule, pid_t parentpid, void (*handler)(int, siginfo_t *, void *), QObject *parent=0);
    ~SignalsModule();

    void shutDownEverything();
    void sendOfflineMode();
    void sendOnlineMode();
    void sendAcknowledge();

    void makeViewImage();
    void buttonNotify();
    void stop();

signals:
    void buttonPressed();
    void newViewimg();
    void log(QString sender, QString message, QString priority);

    void offlineMode();
    void onlineMode();

    void finished();

public slots:
    void enableEmission();
    void disableEmission();

    void disconnectClient();

    void notifyConnectionErrorSender(QString error, int nerr);
    void notifyConnectionErrorHttp(QString error);

    void process();

private:
    bool enabled;
    bool working;
    bool online;

    pid_t daemonPid;

    void notifyDaemon(int msg);
    void goOffline();
    void goOnline();

};

#endif // BUTTONMODULE_H
