#include "TestWiringPi.h"

TestWiringPi::TestWiringPi(QObject *parent) : QObject(parent)
{}

void TestWiringPi::aButtonWasPressed()
{
    printf("Button pressed! ");
    fflush(stdout);
}

