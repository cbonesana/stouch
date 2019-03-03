#include "RFIDInventoryModule.h"

RFIDInventoryModule::RFIDInventoryModule(QString config, RFIDModule *rfidModule, QObject *parent) : Module(config, "RFID Inventory Module", parent)
{
    working = true;
    inventoryStatus = false;
    number = 0;
    viewimg = 0;

    this->rfidModule = rfidModule;
    this->rfidModule->setProtocol(0x00000003);  // set protocol to EPC C1G2
    this->rfidModule->setPower(199);            // set maximum power

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(this, SIGNAL(stopTimer()), timer, SLOT(stop()));
}

void RFIDInventoryModule::stop()
{
    emit stopTimer();
    emit log(componentName, "Inventory process completed.", "SETUP");
    emit finished();
}

void RFIDInventoryModule::process(){
    emit log(componentName, "Start inventory process.", "SETUP");
    timer->start(fileParser->parseFor("inventory_time",INVENTORY_TIME).toInt());
}

void RFIDInventoryModule::update()
{
    RFIDMessage *message;
    if (inventoryStatus){
        number++;
        message = rfidModule->inventory(source, 9);

        if (message != NULL){
            vector<RFID>* founds = message->getRFIDs();
            if (!founds->empty()){
                QString str = QString(RFIDModule::parseRFID(founds->front()));

                emit log(componentName, "Found RFID: " % str % ".", "INFO ");
                emit found(str);
                disableInventory();

            }
            delete(founds);
            delete(message);

        } else {
            emit log(componentName, "Exception in RFID continuous inventory process!", "ERROR");
        }
    }
    if (number == fileParser->parseFor("scan_to_log",SCAN_TO_LOG).toInt()){
        number = 0;
        emit log(componentName, "Log entry every " % fileParser->parseFor("scan_to_log",SCAN_TO_LOG) % " scan(s).", "LOG  ");
        viewimg++;

        if (viewimg == 10){
            viewimg = 0;
            emit newViewImage();
        }
    }
}

void RFIDInventoryModule::enableInventory()
{
    inventoryStatus = true;
    emit log(componentName, "Inventory acquisition enabled.", "INFO ");
}

void RFIDInventoryModule::disableInventory()
{
    inventoryStatus = false;
    emit log(componentName, "Inventory acquisition disabled.", "INFO ");
}
