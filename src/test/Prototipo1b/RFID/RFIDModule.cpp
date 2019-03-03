#include "RFIDModule.h"

RFIDModule::RFIDModule(QString config, SerialModule *module, QObject *parent) : Module(config, "RFID Module", parent)
{
    serialModule = module;
    emit log(componentName, "Initialization... ok!","INFO ");
}

char *RFIDModule::parseRFID(RFID rfid)
{
    char *str = new char[1024]();
    for (short i=0; i<rfid.length; i++){
        sprintf(str,"%s%02x:",str,rfid.rfid[i]);
    }
    str[strlen(str)-1] = '\0';
    return str;
}

RFIDMessage *RFIDModule::sendAndRecieve(RFIDMessage *message)
{
    unsigned char *send = message->getBuffer();
    unsigned int sendLength = message->getLength();
    unsigned int receivedLength;

    unsigned char *received = serialModule->sendAndReceive(send, sendLength, &receivedLength);

    if (receivedLength < 10){
        return NULL;
    }
    return new RFIDMessage(received,receivedLength);
}

RFIDMessage *RFIDModule::getAntennaStatus(unsigned char *source, unsigned short len, unsigned short id)
{
    emit log(componentName, "Check antenna status", "INFO");
    RFIDMessage *msgCheckAntenna = new RFIDMessage(id);
    msgCheckAntenna->addCommand(RFIDAttributeTypes::COMMAND_NAME, RFIDCommandsCodes::CHECK_READ_POINT_STATUS);
    msgCheckAntenna->addCommand(RFIDAttributeTypes::READ_POINT_NAME, source, len);
    RFIDMessage *response = sendAndRecieve(msgCheckAntenna);
    delete(msgCheckAntenna);
    return response;
}

RFIDMessage *RFIDModule::getPower(unsigned short id)
{
    emit log(componentName, "GetPower request.","INFO ");
    RFIDMessage *msgGetPower = new RFIDMessage(id);
    msgGetPower->addCommand(RFIDAttributeTypes::COMMAND_NAME, RFIDCommandsCodes::GET_POWER);
    RFIDMessage *response = sendAndRecieve(msgGetPower);
    delete(msgGetPower);
    return response;
}

RFIDMessage *RFIDModule::setPower(unsigned int powerLevel, unsigned short id)
{
    emit log(componentName, "SetPower request.","INFO ");
    RFIDMessage *msgSetPower = new RFIDMessage(id);
    msgSetPower->addCommand(RFIDAttributeTypes::COMMAND_NAME, RFIDCommandsCodes::SET_POWER);
    msgSetPower->addCommand(RFIDAttributeTypes::POWER_SET, powerLevel);
    RFIDMessage *response = sendAndRecieve(msgSetPower);
    delete(msgSetPower);
    return response;
}

RFIDMessage *RFIDModule::setProtocol(unsigned int protocol, unsigned short id)
{
    emit log(componentName, "SetProtocol request!","INFO ");
    RFIDMessage *msgSetProtocol = new RFIDMessage(id);
    msgSetProtocol->addCommand(RFIDAttributeTypes::COMMAND_NAME, RFIDCommandsCodes::SET_PROTOCOL);
    msgSetProtocol->addCommand(RFIDAttributeTypes::PROTOCOL, protocol);
    RFIDMessage *response = sendAndRecieve(msgSetProtocol);
    delete(msgSetProtocol);
    return response;
}

RFIDMessage *RFIDModule::getProtocol(unsigned short id)
{
    emit log(componentName, "GetProtocol request.","INFO ");
    RFIDMessage *msgGetProtocol = new RFIDMessage(id);
    msgGetProtocol->addCommand(RFIDAttributeTypes::COMMAND_NAME, RFIDCommandsCodes::GET_PROTOCOL);
    RFIDMessage *response = sendAndRecieve(msgGetProtocol);
    delete(msgGetProtocol);
    return response;
}

RFIDMessage *RFIDModule::inventory(unsigned char *source, unsigned short len, unsigned short id)
{
    //emit log(componentName, "Inventory request.","INFO ");
    RFIDMessage *msgInventory = new RFIDMessage(id);
    RFIDMessage *msgIn;
    msgInventory->addCommand(RFIDAttributeTypes::COMMAND_NAME, RFIDCommandsCodes::INVENTORY_TAG);
    msgInventory->addCommand(RFIDAttributeTypes::SOURCE_NAME, source, len);

    msgIn = sendAndRecieve(msgInventory);

    delete(msgInventory);

    return msgIn;
}
