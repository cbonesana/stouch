#include "TestSOund.h"

TestSOund::TestSOund(QObject *parent) :
    QObject(parent)
{
}

void TestSOund::play(QString s)
{
    emit sound(s, 1);
}
