#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include <../Prototipo1b/Log/LogModule.h>
#include <../Prototipo1b/Connection/SocketModule.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    LogModule *logModule = new LogModule("LOG");
    SocketModule *socketModule = new SocketModule();

    QObject::connect(socketModule, SIGNAL(log(QString,QString,QString)), logModule, SLOT(logging(QString,QString,QString)));

    QFile *sourceFile = new QFile(argv[1]);
    bool status = sourceFile->open(QIODevice::ReadOnly);
    if (!status){
        qDebug() << "No image found!" << endl;
        exit(EXIT_FAILURE);
    }
    QByteArray base64EncodedImage = sourceFile->readAll().toBase64();
    sourceFile->close();

    QString dataPart1 = "idUser=15305;comando=VIEWIMG;mac=B8:27:EB:10:47:C3;param=";
    QString dataPart2 = ";risposta=9999";

    QByteArray data = dataPart1.toLocal8Bit();
    data.append(base64EncodedImage);
    data.append(dataPart2.toLocal8Bit());

    socketModule->connectToServer("62.2.177.143", 62001);
    socketModule->write(data);
    qDebug() << socketModule->readAll() << endl;
    socketModule->close();

    return a.exec();
}
