#include <QCoreApplication>

#include <QString>
#include <QThread>

#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Connection/ConnectionImage.h"
#include "Connection/SendModule.h"
#include "Connection/HTTPModule.h"
#include "Log/LogModule.h"
#include "RFID/RFIDInventoryModule.h"
#include "Signals/SignalsHandlers.h"
#include "Raspberrypi.h"
#include "config.h"


using namespace std;

void checkDirectories();
void registerPID();

pid_t parentpid = 0;

int main(int argc, char *argv[])
{
    int arg;
    int debug = false;
    while ((arg = getopt(argc, argv, "p:d")) != -1){
        switch(arg){
        case 'p':       // parent process id
            parentpid = atoi(optarg);
            break;
        case 'd':
            debug = true;
            break;
        case 'h':
            return 0;
        default :
            break;
        }
    }

    QCoreApplication a(argc, argv);

    // Controllo della presenza delle directory
    checkDirectories();

    // inizializzazione logger
    QThread *logThread = new QThread();
    LogModule *logModule = new LogModule(CONFIG_FILE);

    QObject::connect(logModule, SIGNAL(finished()), logThread, SLOT(quit()));
    QObject::connect(logModule, SIGNAL(finished()), logModule, SLOT(deleteLater()));
    QObject::connect(logThread, SIGNAL(finished()), logThread, SLOT(deleteLater()));

    logModule->moveToThread(logThread);
    logThread->start();

    // Inizializzazione modulo segnali
    initSignals(&a, parentpid, logModule);
    QThread *signalsThread = new QThread();
    // registerPID();          // Registra il pid sul server

    //QObject::connect(signalsThread, SIGNAL(started()),  signalsModule, SLOT(process()));
    QObject::connect(signalsModule, SIGNAL(finished()), signalsThread, SLOT(quit()));
    QObject::connect(signalsModule, SIGNAL(finished()), signalsModule, SLOT(deleteLater()));
    QObject::connect(signalsThread, SIGNAL(finished()), signalsThread, SLOT(deleteLater()));

    signalsModule->moveToThread(signalsThread);
    // start delayed

    // inizializzazione suoni
    QThread *soundThread = new QThread();
    SoundModule *soundModule = new SoundModule(CONFIG_FILE, SOUND_CONF);

    QObject::connect(soundModule, SIGNAL(finished()), soundThread, SLOT(quit()));
    QObject::connect(soundModule, SIGNAL(finished()), soundModule, SLOT(deleteLater()));
    QObject::connect(soundThread, SIGNAL(finished()), soundThread, SLOT(deleteLater()));

    QObject::connect(soundModule, SIGNAL(log(QString,QString,QString)), logModule, SLOT(logging(QString,QString,QString)));

    soundModule->moveToThread(soundThread);
    soundThread->start();

    // inizializzazione thread per l'invio di dati via socket
    QQueue<ConnectionImage> *imageQueue = new QQueue<ConnectionImage>();

    QThread *connectionThread = new QThread();
    SendModule *connectionModule = new SendModule(CONFIG_FILE,imageQueue);

    QObject::connect(connectionModule, SIGNAL(connectionErrorSender(QString,int)), signalsModule, SLOT(notifyConnectionErrorSender(QString,int)));
    QObject::connect(connectionModule, SIGNAL(log(QString,QString,QString)),       logModule,     SLOT(logging(QString,QString,QString)));

    QObject::connect(connectionThread, SIGNAL(finished()), connectionThread, SLOT(deleteLater()));
    QObject::connect(connectionThread, SIGNAL(finished()), connectionThread, SLOT(deleteLater()));

    connectionModule->moveToThread(connectionThread);
    // start delayed

    // inizializzazione thread per l'invio di dati al server
    QThread *httpThread = new QThread();
    HTTPModule *httpModule = new HTTPModule(CONFIG_FILE,debug);

    QObject::connect(httpModule, SIGNAL(connectionErrorHttp(QString)), signalsModule, SLOT(notifyConnectionErrorHttp(QString)));
    QObject::connect(httpModule, SIGNAL(log(QString,QString,QString)), logModule,  SLOT(logging(QString,QString,QString)));
    QObject::connect(logModule,  SIGNAL(logMessage(QString)),          httpModule, SLOT(logOnServer(QString)));

    QObject::connect(httpThread, SIGNAL(finished()), httpThread, SLOT(deleteLater()));
    QObject::connect(httpThread, SIGNAL(finished()), httpThread, SLOT(deleteLater()));

    httpModule->moveToThread(httpThread);

    // inizializzazione RaspberryPi

    CameraModule *cameraModule = new CameraModule(CONFIG_FILE);
    SerialModule *serialModule = new SerialModule(CONFIG_FILE,"/dev/ttyUSB0", B115200, 0);
    RFIDModule *rfidModule = new RFIDModule(CONFIG_FILE,serialModule);

    // inizializzazione RFID inventario continuo
    QThread *rfidInventoryThread = new QThread();
    RFIDInventoryModule *rfidInventoryModule = new RFIDInventoryModule(CONFIG_FILE,rfidModule);

    QObject::connect(rfidInventoryModule, SIGNAL(log(QString,QString,QString)), logModule, SLOT(logging(QString,QString,QString)));
    QObject::connect(rfidInventoryThread, SIGNAL(started()),  rfidInventoryModule, SLOT(process()));
    QObject::connect(rfidInventoryModule, SIGNAL(finished()), rfidInventoryThread, SLOT(quit()));
    QObject::connect(rfidInventoryModule, SIGNAL(finished()), rfidInventoryModule, SLOT(deleteLater()));
    QObject::connect(rfidInventoryThread, SIGNAL(finished()), rfidInventoryThread, SLOT(deleteLater()));

    rfidInventoryModule->moveToThread(rfidInventoryThread);
    rfidInventoryThread->start();

    RaspberryPi *raspberryPi = RaspberryPi::create(CONFIG_FILE, imageQueue, cameraModule, signalsModule, soundModule, rfidModule, rfidInventoryModule);

    QObject::connect(raspberryPi, SIGNAL(log(QString,QString,QString)), logModule, SLOT(logging(QString,QString,QString)));

    QObject::connect(httpModule,  SIGNAL(sigCheckUserDet(QString, QString)),    raspberryPi, SLOT(setCheckUserDet(QString, QString)));
    QObject::connect(httpModule,  SIGNAL(sigSetRFID(QString,QString)),          raspberryPi, SLOT(manageRFIDFound(QString,QString)));
    QObject::connect(httpModule,  SIGNAL(sigGetCrop(QString)),                  raspberryPi, SLOT(manageCropImage(QString)));
    QObject::connect(httpModule,  SIGNAL(sigBackScan()),                        raspberryPi, SLOT(manageBackScan()));

    QObject::connect(raspberryPi, SIGNAL(contactHttpServer(QString,QString,QString,QString,QString,QString)), httpModule, SLOT(contactServer(QString,QString,QString,QString,QString,QString)));

    QObject::connect(httpModule,  SIGNAL(sigTestRFID(QString,QString)), connectionModule, SLOT(manageTestRFID(QString,QString)));
    QObject::connect(httpModule,  SIGNAL(sigTestCrop(QString)),         connectionModule, SLOT(manageTestCrop(QString)));

    QObject::connect(raspberryPi,      SIGNAL(startSendTask()), connectionModule, SLOT(startTask()));
    QObject::connect(connectionModule, SIGNAL(taskStarted()),   raspberryPi,      SLOT(stopTask()));
    QObject::connect(connectionModule, SIGNAL(taskCompleted()), raspberryPi,      SLOT(startTask()));
    QObject::connect(connectionModule, SIGNAL(externalCropImage(QString,QString,QString)), raspberryPi, SLOT(manageExternalCropImage(QString,QString,QString)));
    QObject::connect(connectionModule, SIGNAL(socketSendFinished()), raspberryPi, SLOT(manageSocketFinished()));

    QObject::connect(connectionModule, SIGNAL(contactHttpServer(QString,QString,QString,QString,QString,QString)), httpModule, SLOT(contactServer(QString,QString,QString,QString,QString,QString)));

    QObject::connect(signalsModule, SIGNAL(offlineMode()), httpModule,       SLOT(goOfflineMode()));
    QObject::connect(signalsModule, SIGNAL(offlineMode()), connectionModule, SLOT(goOfflineMode()));
    QObject::connect(signalsModule, SIGNAL(offlineMode()), raspberryPi,      SLOT(goOfflineMode()));
//    QObject::connect(signalsModule, SIGNAL(newViewimg()),  raspberryPi,      SLOT(firstPhoto()));

    QObject::connect(signalsModule, SIGNAL(onlineMode()), httpModule,        SLOT(goOnlineMode()));
    QObject::connect(signalsModule, SIGNAL(onlineMode()), connectionModule,  SLOT(goOnlineMode()));
    QObject::connect(signalsModule, SIGNAL(onlineMode()), raspberryPi,       SLOT(goOnlineMode()));

    signalsThread->start();
    connectionThread->start();
    httpThread->start();
    raspberryPi->start();

    return a.exec();
}

void checkDirectories(){
    struct stat st;

    // directory per le immagini
    if (stat("img/", &st) == -1) {
        mkdir("img", 0744);
    }
    if (stat("crop/", &st) == -1) {
        mkdir("crop", 0744);
    }
}

void registerPID(){
    int fp = open(CLIENT_PID_FILE, O_WRONLY);
    char str[255];
    sprintf(str,"%d\n", getpid());
    write(fp, str, strlen((str)));
    close(fp);
}
