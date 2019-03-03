#include "SocketModule.h"

/*!
 * \brief SocketModule::SocketModule
 * \param parent
 */
SocketModule::SocketModule(QString config, QObject *parent) : Module(config, "Socket Module", parent)
{
    socket = new QTcpSocket(parent);
    connect(socket, SIGNAL(connected()),    this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    emit log(componentName, "configured.", "SETUP");
}

/*!
 * \brief SocketModule::~SocketModule
 */
SocketModule::~SocketModule()
{
    if (socket != NULL){
        socket->disconnectFromHost();
        socket->close();
        delete(socket);
    }
}
/*!
 * \fn SocketModule::socketError
 */

/*!
 * \fn SocketModule::finished
 */

/*!
 * \fn SocketModule::connectionSuccessfull
 */

/*!
 * \fn SocketModule::connectionErrorSocket
 * \param error
 * \param nerr
 */

/*!
 * \fn SocketModule::connectToServer
 * \param server
 * \param port
 */
void SocketModule::connectToServer(QString server, quint16 port)
{
    QHostAddress address(server);

    socket = new QTcpSocket();

    socket->connectToHost(address, port);
    emit log(componentName, "connection to server " % server % ":" % QString::number(port) % ".", "INFO ");
}

/*!
 * \brief SocketModule::connected
 */
void SocketModule::connected()
{
    emit log(componentName, "connection succesfully.", "INFO ");
    emit connectionSuccessfull();
}

/*!
 * \brief SocketModule::disconnected
 */
void SocketModule::disconnected()
{
    emit log(componentName, "disconnection succesfully.", "INFO ");
    emit finished();
    close();
}

/*!
 * \brief SocketModule::error
 * \param err
 */
void SocketModule::error(QAbstractSocket::SocketError err)
{
    emit log(componentName, "socket error: " % socket->errorString() % ".", "ERROR");
    emit connectionErrorSocket(socket->errorString(), err);
}

/*!
 * \brief SocketModule::write
 * \param data
 */
void SocketModule::write(QString data)
{
    write(data.toLocal8Bit());
}

/*!
 * \brief SocketModule::write
 * \param data
 */
void SocketModule::write(QByteArray data)
{
    int size = socket->write(data);
    log(componentName, "Sent " % QString::number(size) % " of " % QString::number(data.size()) % ".", "INFO ");
}

/*!
 * \brief SocketModule::readAll
 * \return
 */
QByteArray SocketModule::readAll()
{
    return socket->readAll();
}

/*!
 * \brief SocketModule::close
 */
void SocketModule::close()
{
    socket->close();
    emit log(componentName, "socket closed.", "INFO ");
}
