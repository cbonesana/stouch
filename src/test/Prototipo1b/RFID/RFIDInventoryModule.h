#ifndef RFIDINVENTORYMODULE_H
#define RFIDINVENTORYMODULE_H

#include <QObject>
#include <QString>
#include <QTimer>

#include <QDebug>
#include <vector>

#include "RFIDModule.h"
#include "RFIDMessage.h"
#include "../Module.h"
#include "../config.h"

using namespace std;

class RFIDInventoryModule : public Module
{
    Q_OBJECT

public:
    RFIDInventoryModule(QString config, RFIDModule *rfidModule, QObject *parent=0);

public slots:
    void stop();
    void update();
    void process();

    void enableInventory();
    void disableInventory();

signals:
    void found(QString rfid);
    void stopTimer();
    void finished();
    void error();
    void newViewImage();

private:
    bool working;
    bool inventoryStatus;

    int number;
    int viewimg;

    unsigned char source[9] = {0x53, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x5F, 0x30, 0x00};

    QTimer *timer;
    RFIDModule *rfidModule;
};

#endif // RFIDINVENTORYMODULE_H
