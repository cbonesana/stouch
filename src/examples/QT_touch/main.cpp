#include <QCoreApplication>

#include <serialthread.h>
#include <signalpool.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    char *filename;

    if (argc < 2){
        filename = "/dev/ttyACM0";
    } else {
        filename = argv[1];
    }
    
    SerialThread *serialThread = new SerialThread(filename);
    SignalPool *signalPool = new SignalPool(serialThread);

    serialThread->start();
    serialThread->wait();

    return a.exec();
}
