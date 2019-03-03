#include "SignalsModule.h"

SignalsModule::SignalsModule(QString config, LogModule *logModule, pid_t parentpid, void (*handler)(int, siginfo_t *, void *), QObject *parent) : Module(config, "Signals Module", parent)
{
    enabled = true;
    working = true;
    online  = true;

    struct sigaction act;
    act.sa_flags = SA_SIGINFO ;
    act.sa_sigaction = handler;

    sigaction(SIGUSR1, &act, NULL);

    daemonPid = parentpid;

    connect(this, SIGNAL(log(QString,QString,QString)), logModule, SLOT(logging(QString,QString,QString)));

    emit log(componentName, "Server pid: " % QString::number(daemonPid) % ".", "SETUP");

    union sigval value;
    value.sival_int = D_REGIST_CLIENT;

    if (kill(daemonPid, 0) == 0){
    if (sigqueue(daemonPid,SIGUSR1,value) != 0) {
        emit log(componentName, "Error with signal " % QString::number(D_REGIST_CLIENT) % " to server: " % QString::number(daemonPid) % ".", "ERROR");
    } else {
        emit log(componentName, "Emit signal " % QString::number(D_REGIST_CLIENT) % " to server: " % QString::number(daemonPid) % ".", "SETUP");
    }
    } else {
        emit log(componentName, "Process " % QString::number(daemonPid) % " doesn't exist.", "ERROR");
    }
}

SignalsModule::~SignalsModule()
{}

void SignalsModule::shutDownEverything()
{
    emit log(componentName, "Received shut down signal", "STOP ");
}

void SignalsModule::sendOfflineMode()
{
    if (online){
        online = false;
        emit log(componentName, "Received offline mode signal", "OFF  ");
        emit offlineMode();
    }
}

void SignalsModule::sendOnlineMode()
{
    if (!online){
        online = true;
        emit log(componentName, "Received online mode signal", "ON   ");
        emit onlineMode();
    }
}

void SignalsModule::sendAcknowledge()
{
    emit log(componentName, "Received acknowledge signal.", "INFO ");
    notifyDaemon(D_ACKNOWLEDGE);
}

void SignalsModule::makeViewImage()
{
    emit log(componentName, "Received buttons signal. Emit VIEWIMG.", "INFO ");
    emit newViewimg();
    notifyDaemon(D_ACKNOWLEDGE);
}

void SignalsModule::buttonNotify()
{
    if (enabled){
        emit log(componentName, "Button pressed!", "INFO ");
        emit buttonPressed();
    }
}

void SignalsModule::stop()
{
    working = false;
}

void SignalsModule::enableEmission()
{
    enabled = true;
    emit log(componentName, "Buttons enabled.", "INFO ");
}

void SignalsModule::disableEmission()
{
    enabled = false;
    emit log(componentName, "Buttons disabled.", "INFO ");
}

void SignalsModule::disconnectClient()
{
    //TODO
    //notifyDaemon(D_REMOVE_CLIENT);
}

void SignalsModule::notifyConnectionErrorSender(QString error, int nerr)
{
    emit log(componentName, "notify connection error from sender: " % error % " (" % QString::number(nerr) % ").", "ERROR");
    notifyDaemon(D_ERROR);
    goOffline();
}

void SignalsModule::notifyConnectionErrorHttp(QString error)
{
    emit log(componentName, "notify connection error from http manager: " % error % ".", "ERROR");
    notifyDaemon(D_ERROR);
    goOffline();
}

void SignalsModule::process()
{
    while(working){
        sleep(1);
    }
    emit finished();
}

void SignalsModule::notifyDaemon(int msg)
{
    union sigval value;
    value.sival_int = msg;

    if (sigqueue(daemonPid,SIGUSR1,value) != 0){
        emit log(componentName, "Error with signal " % QString::number(msg) % " to server: " % QString::number(daemonPid) % ".", "ERROR");
    } else {
        emit log(componentName, "Emit signal " % QString::number(msg) % " to server: " % QString::number(daemonPid) % ".", "INFO ");
    }
}

void SignalsModule::goOffline()
{
    emit log(componentName, "start offline mode.", "OFF  ");
    emit offlineMode();
}

void SignalsModule::goOnline()
{
    emit log(componentName, "back to online mode.", "ON   ");
    emit onlineMode();
}
