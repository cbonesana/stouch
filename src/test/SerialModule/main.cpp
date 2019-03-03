#include <QCoreApplication>
#include <QThread>

#include "../Prototipo1b/Serial/SerialModule.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QThread *thread = new QThread();
    SerialModule *serial = new SerialModule("/dev/tty1");



    return a.exec();
}
