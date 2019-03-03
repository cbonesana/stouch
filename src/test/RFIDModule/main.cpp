#include <QCoreApplication>

#include <QThread>

#include "../Prototipo1b/RFID/RFIDMessage.h"
#include "../Prototipo1b/RFID/RFIDModule.h"
#include "../Prototipo1b/RFID/RFIDCommands.h"

#include "../Prototipo1b/RFID/RFIDattributeTypes.h"
#include "../Prototipo1b/RFID/RFIDCommandsCodes.h"

#include "../Prototipo1b/Serial/SerialModule.h"
#include "../Prototipo1b/Log/LogModule.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <errno.h>

#include <iostream>
#include <iomanip>

#include <math.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc,argv);

    LogModule *log = new LogModule("RFID");
    QThread *thread = new QThread();

    SerialModule *serial = new SerialModule("/dev/ttyUSB0", B115200, 0);

    RFIDModule *rfid = new RFIDModule(serial);
    RFIDMessage *msgIn;

    QObject::connect(serial, SIGNAL(log(QString,QString,QString)), log, SLOT(log(QString,QString,QString)));

    QObject::connect(thread, SIGNAL(finished()), log, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // values
    unsigned char Source_0[9] = {0x53, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x5F, 0x30, 0x00};
    unsigned int protocol     = 0x00000003;

    unsigned int powerInitial = 199;
    unsigned int powerMiddle  = 80;
    unsigned int powerTesting = 19;

    double gain = 8.0;
    double loss = 1.5;
    double ERPPower = 2000.0;
    unsigned int powerTest1   = (int)(ERPPower/pow(10,((gain-loss-2.14)/10)));

    printf("POWER: %x \n", powerTest1);

    // Set Protocol
//    msgIn = rfid->setProtocol(protocol);
////    msgIn->print();
//    getc(stdin);


    // Get Power
//    msgIn = rfid->getPower();
//    msgIn->print();
//    printf("GET POWER COMPLETE\n");
//    getc(stdin);

//    // Set Power
    msgIn = rfid->setPower(powerInitial);
    msgIn->print();
    printf("SET POWER COMPLETE\n");
    getc(stdin);

//    // Get Power
    msgIn = rfid->getPower();
    msgIn->print();
    printf("GET POWER COMPLETE\n");
    getc(stdin);

    // antenna status
//    msgIn = rfid->getAntennaStatus(Source_0, 9);
//    msgIn->print();
//    getc(stdin);

    printf("START INVENTORY\n");

    // Inventory
    try {
        printf("\n");

        forever {
            try {
                msgIn = rfid->inventory(Source_0,9);
            } catch(RFIDException& e){
                std::cout << e.what() << std::endl;
            }

            int k=0;
            try {
                vector<RFID> *founds = msgIn->getRFIDs();

                for(vector<RFID>::iterator it = founds->begin(); it != founds->end(); ++it){
                    RFID t = *it;
                    printf("%2d] \t",k);
                    for (short i=0; i<t.length; i++){
                        printf("%02x ",t.rfid[i]);
                    }
                    printf("\n");
                    k++;
                }
                printf("\n");
            } catch (RFIDException& e) {
                printf ("Nothing found.\n");
            }
            sleep(1);
        }
    } catch (RFIDException& e) {
        printf("Eccezione!\n");
        std::cout << e.what() << std::endl;
    }

    printf("\n");

    return 0;
}
