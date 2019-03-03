#include <QCoreApplication>

#include <QObject>

#include <cstdio>

#include "../Prototipo1b/Buttons/ButtonInit.h"
#include "TestWiringPi.h"

using namespace std;

TestWiringPi *tester;

int main (int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    initButtons();
    tester = new TestWiringPi();

    QObject::connect(buttonModule, SIGNAL(buttonPressed()), tester, SLOT(aButtonWasPressed()));

    printf("Start test operations.\n");

    return a.exec();
}

