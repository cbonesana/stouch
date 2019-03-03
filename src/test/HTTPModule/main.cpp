#include <QCoreApplication>

#include <QNetworkReply>
#include <QDebug>

#include "../Prototipo1b/Connection/HTTPManager.h"
#include "../Prototipo1b/Connection/HTTPResponse.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    HTTPManager *manager = new HTTPManager();

    QNetworkReply *reply = manager->contactServer(
                "http://www.tv-surf.com/3D-Enter/servlet/P13_19_CommandXML",
                "230",
                "DEBUG",
                "",
                "test",
                "salbadan");

    return a.exec();
}
