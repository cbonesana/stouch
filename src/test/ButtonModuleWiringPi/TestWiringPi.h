#ifndef TESTWIRINGPI_H
#define TESTWIRINGPI_H

#include <QObject>

#include <cstdio>

class TestWiringPi : public QObject
{
    Q_OBJECT

public:
    explicit TestWiringPi(QObject *parent = 0);

public slots:
    void aButtonWasPressed(void);
    
};

#endif // TESTWIRINGPI_H
