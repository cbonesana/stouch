#include "IOModule.h"

IOModule::IOModule(QObject *parent) : QObject(parent)
{}

void IOModule::handleError(QString err)
{
    printf("Error: %s \n", qPrintable(err));
}

void IOModule::handleEvent()
{
    printf("Log has finished.\n");
}

void IOModule::process()
{
    char buff;

    do {

        std::cout << "\n> ";
        std::cin >> buff;

        switch(buff) {
        case 'a':
            emit logData("A", "Message from A", "PrioA");
            break;
        case 'b':
            emit logData("B", "Message from B", "PrioB");
            break;
        default:
            ;
        }

    } while (buff != 'q');
    emit finished();
}
