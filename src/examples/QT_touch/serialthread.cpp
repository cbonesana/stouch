#include "serialthread.h"

SerialThread::SerialThread(const char *device, QObject *parent) : QThread(parent)
{
    deviceName = device;
}

void SerialThread::start()
{
    run();
}

void SerialThread::run()
{
    device = open(deviceName, O_RDWR | O_NOCTTY | O_SYNC);
    char buffer[1024];

    if (device < 0){
        qDebug() << "Error " << errno << " opening " << deviceName << ": " << strerror(errno) << endl;
        return;
    }

    setInterfaceAttribs(device, B9600, 0);
    setBlocking(device, true);

    forever {
        int n = read(device, buffer, sizeof(buffer));
        if (n > 0)
        {
            emit dataReceived();
            emit charReceived(buffer[0]);
        }
    }
}

int SerialThread::setInterfaceAttribs(int fd, int speed, int parity)
{
    memset (&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0)
    {
        qDebug() << "Error " << errno << " from tcgetattr" << endl;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-biz characters

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

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        qDebug() << "Error " << errno << " from tcsetattr" << endl;
        return -1;
    }

    return 0;
}

int SerialThread::setBlocking(int fd, bool shouldBlock)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        qDebug() << "Error " << errno << " from tggetattr" << endl;
        return -1;
    }

    tty.c_cc[VMIN]  = shouldBlock ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        qDebug() << "Error " << errno << " setting term attributes" << endl;
        return -1;
    }
    return 0;
}
