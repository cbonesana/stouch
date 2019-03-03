#include "SerialModule.h"

SerialModule::SerialModule(QString config, QString device, QObject *parent) : Module(config, "Serial Module", parent)
{
    deviceName = device;
    working = true;
}

SerialModule::SerialModule(QString config, QString device, int speed, int parity, QObject *parent) : Module(config, "Serial Module", parent)
{
    deviceName = device;
    working = true;

    if (init(speed, parity)){
        emit log(componentName, "Serial initialization completed.", "SETUP");
    } else {
        emit log(componentName, "Serial initialization failed!", "ERROR");
    }
}

SerialModule::~SerialModule()
{}

bool SerialModule::init(int speed, int parity)
{
    device = open(deviceName.toStdString().c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (device < 0) {
        QString error = "Error" % QString::number(errno) % "opening /dev/ttyUSB0 ";
        emit log(componentName, error, "ERROR");

        emit serialerror(error);
        return false;
    }
    this->setInterfaceAttribs(device, speed, parity);
    return true;
}

int SerialModule::send(unsigned char *inBuffer, unsigned int length)
{
    return write(device, inBuffer, length);
}

int SerialModule::receive(unsigned char *outBuffer, unsigned int length)
{
    return read(device, outBuffer, length);
}

unsigned char *SerialModule::sendAndReceive(unsigned char *sendBuffer, unsigned int sendLength, unsigned int *receivedLength)
{

    int status = 0;
    status = send(sendBuffer,sendLength);
    emit log(deviceName, "send & receive: sent " % QString::number(status) % " bytes.", "INFO ");

    unsigned char *receivedBuffer = new unsigned char[MAX_BUFFER];
    unsigned int pointer = 0;

    do {
        status = receive(&receivedBuffer[pointer], sizeof(unsigned char));
        if (status == 1) {
            pointer++;
        }
    } while (status == 1);

    *receivedLength = pointer;

    emit log(deviceName, "send & receive: received " % QString::number(status) % " bytes.", "INFO ");

    return receivedBuffer;
}

void SerialModule::process()
{
    emit log(componentName,"Process started", "INFO ");

    device = open(deviceName.toStdString().c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    char buffer[1024];

    if (device < 0) {
        QString error = "Error " % QString::number(errno) % " opening " % deviceName % ": " % QString(strerror(errno));
        emit log(deviceName, error, "ERROR");
        printf("%s\n", qPrintable(error));

        emit finished();

        // TODO: provare a rilanciare il processo?
        // signal o exception?
        return;
    }

    setInterfaceAttribs(device, B9600, 0);
    setBlocking(device, true);

    do {
        int n = read(device, buffer, sizeof(buffer));
        if (n > 0) {
            emit dataReceived();
            emit charReceived(buffer[0]);
        }
    } while(working);

    emit finished();
}

void SerialModule::stop()
{
    working = false;
}

int SerialModule::setInterfaceAttribs(int fd, int speed, int parity)
{
    memset (&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0) {
        QString error = "Error " % QString::number(errno) % " from tcgetattr";
        emit log(componentName,error,"ERROR");
        printf("%s\n", qPrintable(error));

        //TODO: signal or exception?
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit characters

    tty.c_iflag &= ~IGNBRK;     // ignore break signal
    tty.c_lflag = 0;            // no signaling char, no echo, no canonical processing
    tty.c_oflag = 0;            // no remapping, no delays
    tty.c_cc[VMIN]  = 0;        // read doesn't block
    tty.c_cc[VTIME] = 5;        // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        QString error = "Error " % QString::number(errno) % " from tcsetattr";
        emit log(componentName,error,"ERROR");
        printf("%s\n", qPrintable(error));

        //TODO: signal or exception?
        return -1;
    }

    return 0;
}

int SerialModule::setBlocking(int fd, bool shouldBlock)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        QString error = "Error " % QString::number(errno) % " from tcgetattr";
        emit log(componentName,error,"ERROR");
        printf("%s\n", qPrintable(error));

        //TODO: signal or exception?
        return -1;
    }

    tty.c_cc[VMIN]  = shouldBlock ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        QString error = "Error " % QString::number(errno) % " from tcsetattr";
        emit log(componentName,error,"ERROR");
        printf("%s\n", qPrintable(error));

        //TODO: signal or exception?
        return -1;
    }
    return 0;
}
