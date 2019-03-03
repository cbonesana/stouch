#include "HTTPManager.h"

/*!
 * \fn HTTPManager::HTTPManager
 *
 * Constructs the module, create an internal QNetworkAccessManager object, who will be used to contact the server asynchronously.
 *
 * \param parent
 */
HTTPManager::HTTPManager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

/*!
 * \fn HTTPManager::~HTTPManager
 *
 * Destruct the module
 */
HTTPManager::~HTTPManager()
{
    delete(manager);
}

/*!
 * \fn HTTPManager::responseAvailable
 *
 * The signal HTTPManager::responseAvailable is used to share the response from the server with other modules.
 * The parameter \a item contains the name of the JSON response from the server, \a replay the values.
 */

/*!
 * \fn HTTPManager::connectionError
 *
 * The signal HTTPManager::connectionError notify a connection error: the parameter \a error contains the error number.
 */

/*!
 * \fn HTTPManager::contactServer
 *
 * The method HTTPManager::contactServer perform a GET requesto to a given \a host.
 *
 * \param host a valid url to contact.
 * \return a pointer to a QNetworkReply object (not used).
 */
QNetworkReply *HTTPManager::contactServer(QString host)
{
    return manager->get(QNetworkRequest(QUrl(host)));
}

/*!
 * \fn HTTPManager::contactServer
 *
 * The method HTTPManager::contactServer is a specified method for the server. It take several \a parameters to compose a valid url.
 *
 * \param host a valid name of the web server
 * \param idUser device id
 * \param command command to execute on the server
 * \param mac mac address or special id
 * \param param various parameters
 * \param risposta name of the JSON object in the answear
 * \return a pointer to a QNetworkReply object (not used).
 */
QNetworkReply *HTTPManager::contactServer(QString host, QString idUser, QString command, QString mac, QString param, QString risposta)
{
    QString address = host % "idUser=" % idUser % "&comando=" % command % "&mac=" % mac % "&param=" % param % "&risposta=" % risposta;
    return contactServer(address);
}

/*!
 * \fn HTTPManager::getMacAddress
 * \return the FIRST mac address available, who is not a loopback interface. It can return a Ethernet mac address!
 */
QString HTTPManager::getMacAddress()
{
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
        // ritorna solamente il primo MAC Address che non sia di loopback
        if (!netInterface.flags().testFlag(QNetworkInterface::IsLoopBack) && netInterface.flags().testFlag(QNetworkInterface::IsUp)) {
            return netInterface.hardwareAddress();
        }
    }
    return QString();
}

/*!
 * \fn HTTPManager::getIpAddress
 * \return the current ip address of the user network interface.
 */
QString HTTPManager::getIpAddress()
{
    foreach(QHostAddress hostAddress, QNetworkInterface::allAddresses()) {
        if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol && hostAddress != QHostAddress(QHostAddress::LocalHost))
            return hostAddress.toString();
    }
    return "";
}

/*!
 * \fn HTTPManager::replyFinished
 *
 * The slot HTTPManager::replyFinished parse the server reply. If there is an error, it emits a HTTPManager::connectionError signals, otherwise it parse the reply and emit a HTTPManager::responseAvailable signal.
 *
 * \param reply the reply from the QNetworkAccessManager.
 */
void HTTPManager::replyFinished(QNetworkReply *reply)
{
    // lettura degli attributi della risposta

    // controllo degli errori
    if (reply->error() == QNetworkReply::NoError) {
        // lettura dei dati dalla risposta
        QByteArray data = reply->readAll();

        // isolamento della porzione di testo da parsare
        int start = data.indexOf("(")+1;
        int stop = data.indexOf(")");
        QByteArray captured = data.mid(start,(stop - start));

        HTTPResponse *response = new HTTPResponse(captured);

        emit responseAvailable(QString(data.mid(0,(start -1))), response);
    } else {
        emit connectionError(reply->errorString());
    }
    reply->deleteLater();
}
