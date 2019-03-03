#include <QCoreApplication>

#include <QThread>

#include "../Prototipo1b/Log/LogModule.h"
#include "IOModule.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LogModule *log = new LogModule();
    QThread *thread1 = new QThread();
    log->moveToThread(thread1);

    IOModule *iomodule = new IOModule();
    QThread *thread2 = new QThread();
    iomodule->moveToThread(thread2);

    QObject::connect(iomodule, SIGNAL(logData(QString,QString,QString)), log, SLOT(log(QString,QString,QString)));
    QObject::connect(log, SIGNAL(error(QString)), iomodule, SLOT(handleError(QString)));

    QObject::connect(thread1, SIGNAL(started()), iomodule, SLOT(handleEvent()));
    QObject::connect(thread1, SIGNAL(finished()), log, SLOT(deleteLater()));
    QObject::connect(thread1, SIGNAL(finished()), thread1, SLOT(deleteLater()));

    QObject::connect(thread2, SIGNAL(started()), iomodule, SLOT(process()));
    QObject::connect(iomodule, SIGNAL(finished()), thread2, SLOT(quit()));
    QObject::connect(iomodule, SIGNAL(finished()), iomodule, SLOT(deleteLater()));
    QObject::connect(thread2, SIGNAL(finished()), thread2, SLOT(deleteLater()));

    thread1->start();
    thread2->start();

    return a.exec();
}
