#ifndef COMMANAGER_H
#define COMMANAGER_H

#include <QObject>
#include <QString>
#include <QStringBuilder>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#include "../Module.h"

/*!
 * \class SocketModule
 * \brief The SocketModule class is used to open and manage a QTCPSocket.
 */
class SocketModule : public Module
{
    Q_OBJECT
public:
    SocketModule(QString config, QObject *parent = 0);
    ~SocketModule();

    void connectToServer(const QString server, quint16 port);
    void write(QString data);
    void write(QByteArray data);
    QByteArray readAll();

signals:
    void socketError();
    void finished();

    void connectionSuccessfull();
    void connectionErrorSocket(QString error, int nerr);

public slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError err);
    void close();

private:
    QTcpSocket *socket;

};

#endif // COMMANAGER_H
