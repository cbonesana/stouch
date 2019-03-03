#include "SendModule.h"

/*!
 * \fn SendModule::SendModule
 *
 * Construct a SendModule with an associated queue.
 *
 * \param config path to the external configuration file.
 * \param queue the queue where the images are stored.
 */
SendModule::SendModule(QString config, QQueue<ConnectionImage> *queue) : Module(config, "SendModule")
{
    queueData    = queue;
    socketModule = NULL;
    onlineMode   = true;

    connect(this, SIGNAL(internalTaskStart()), this, SLOT(startTask()));
}

/*!
 * \fn SendModule::~SendModule
 *
 * Destructs the module.
 */
SendModule::~SendModule()
{
    if(socketModule != NULL){
        delete(socketModule);
    }
}

/*!
 * \fn SendModule::taskStarted
 *
 * The signal SendModule::taskStarted is used to notify that the work on the queue is started.
 */

/*!
 * \fn SendModule::taskCompleted
 *
 * The signal SendModule::taskCompleted is used to notify that the queue is empty and every task is complete.
 */

/*!
 * \fn SendModule::internalTaskStart
 *
 * The signal SendModule::internalTaskStart is used to continue working on the queue.
 */

/*!
 * \fn SendModule::contactHttpServer
 *
 * The signal SendModule::contactHttpServer is used to contact the server.
 *
 * \param host a valid name of the web server
 * \param idUser device id
 * \param command command to execute on the server
 * \param mac mac address or special id
 * \param param various parameters
 * \param risposta name of the JSON object in the answear
 */

/*!
 * \fn SendModule::externalCropImage
 *
 * The signal SendModule::externalCropImage is used to request a crop for an image.
 *
 * \param data crop parameters.
 * \param filename photo name.
 * \param userId user id for the photo name.
 */

/*!
 * \fn SendModule::socketSendFinished
 *
 * The signal SendModule::socketSendFinished is used to notify a successful processing on an image.
 */

/*!
 * \fn SendModule::connectionErrorSender
 *
 * The signal SendModule::connectionErrorSender is used to notify a connection error.
 *
 * \param error human readable error.
 * \param nerr machine readable error.
 */

/*!
 * \fn SendModule::readImageFileError
 *
 * The signal SendModule::readImageFileError is used to notify that the module can't open a file.
 */

/*!
 * \fn SendModule::startTask
 *
 * The slot SendModule::startTask is used to process an image in the queue.
 * The first time, this slot create a SocketModule used to send the file to the server.
 */
void SendModule::startTask()
{
    if (socketModule == NULL){
        socketModule = new SocketModule(CONFIG_FILE);

        connect(socketModule, SIGNAL(connectionErrorSocket(QString,int)), this, SLOT(manageSocketError(QString,int)));
        connect(socketModule, SIGNAL(log(QString,QString,QString)),       this, SIGNAL(log(QString,QString,QString)));
        connect(socketModule, SIGNAL(finished()),                         this, SLOT(manageSocketFinished()));

        emit log(componentName, "SocketModule created.", "SETUP");
    }

    emit taskStarted();

        if (isConnectionAvailable()){
            if (queueData->isEmpty()){
                // nothing to do
                emit socketSendFinished();
                return;
            }
            ConnectionImage connectionImage = queueData->head();

            if (connectionImage.rfid == ""){
                // normal mode
                emit log(componentName, "Send image to server: device " % connectionImage.deviceId % " (" % connectionImage.macAddress % ") userid: " % connectionImage.userId % " (" % connectionImage.command % ").", "INFO ");

                QFile *sourceFile = new QFile(connectionImage.filename);
                bool status = sourceFile->open(QIODevice::ReadOnly);
                if (!status){
                    emit log(componentName, "Failed to open source file " % connectionImage.filename % ".", "ERROR");
                    emit readImageFileError();
                    return;
                }
                QByteArray base64EncodedImage = sourceFile->readAll().toBase64();
                sourceFile->close();

                QString dataPart1 = "idUser=" % connectionImage.deviceId % ";comando=" % connectionImage.command % ";mac=" % connectionImage.macAddress % ";param=";
                QString dataPart2 = ";risposta=" % connectionImage.userId % ";";

                QByteArray data = dataPart1.toLocal8Bit();
                data.append(base64EncodedImage);
                data.append(dataPart2.toLocal8Bit());

                emit log(componentName, "Socket executing command: " % connectionImage.command % ".", "INFO ");

                QString serverIp = fileParser->parseFor("server_ip", SERVER_IP);
                QString serverPort = fileParser->parseFor("server_port", SERVER_PORT);

                socketModule->connectToServer(serverIp, serverPort.toUShort());
                socketModule->write(data);
                socketModule->close();

                queueData->dequeue(); // saefly remove

            } else {
                // after reconnection mode
                emit log(componentName, "SENDER HTTP executing command: SET_RFID (" % connectionImage.deviceId % "," % connectionImage.rfid % ").", "INFO ");
                emit contactHttpServer(
                            fileParser->parseFor("server_name", SERVER_NAME) % "?",
                            connectionImage.deviceId,
                            "SET_RFID",
                            connectionImage.rfid,
                            "",
                            "testRFID");
                // do not continue to the next image
                return;
            }
        }

}

/*!
 * \fn SendModule::goOfflineMode
 *
 * The slot SendModule::goOfflineMode is used to switch to offline mode.
 */
void SendModule::goOfflineMode()
{
    onlineMode = false;
    emit log(componentName, "I'm in offline mode now.", "OFF  ");
}

/*!
 * \fn SendModule::goOnlineMode
 *
 * The slot SendModule::goOnlineMode is used to switch to online mode and start working on the queue.
 */
void SendModule::goOnlineMode()
{
    onlineMode = true;
    emit log(componentName, "I'm back in online mode now.", "ON   ");
    emit internalTaskStart();
}

/*!
 * \fn SendModule::manageSocketError
 *
 * The slot SendModule::manageSocketError manages a connection error. It thest if the connection is available.
 * If it is available, it restart the processing on the queue, otherwise it notify the problem with the network.
 *
 * \param error human readable error.
 * \param nerr machine readable error.
 */
void SendModule::manageSocketError(QString error, int nerr)
{
    if (isConnectionAvailable()){
        // false alarm, continue working
        emit internalTaskStart();
    } else {
        emit connectionErrorSender(error, nerr);
    }
}

/*!
 * \fn SendModule::manageSocketFinished
 *
 * The slot SendModule::manageSocketFinished check if the queue is empty.
 * If true, it signals the end of the work, otherwise it launches a new task on the queue.
 */
void SendModule::manageSocketFinished()
{
    if (queueData->isEmpty()){
        // all work is done
        emit socketSendFinished();
    } else {
        // else continue working with next item
        emit internalTaskStart();
    }
}

/*!
 * \fn SendModule::manageTestRFID
 *
 * The slot SendModule::manageTestRFID manage if an rfid is valid. If true, it request the crop data to the server.
 *
 * \param id id of the user.
 * \param error status of the response. If OK, there is a user connected with the rfid, otherwise no.
 */
void SendModule::manageTestRFID(QString id, QString error)
{
    ConnectionImage image = queueData->head();
    emit log(componentName, "managing command SET_RFID, errore = " % error % ", risposta = " % id % ".", "INFO ");
    if (error == "OK"){
        emit log(componentName, "socket executing command: GET_CROP.", "INFO ");
        emit contactHttpServer(
                    fileParser->parseFor("server_name", SERVER_NAME) % "?",
                    image.deviceId,
                    "GET_CROP",
                    image.deviceId,
                    "",
                    "testCrop"
                    );
    } else {
        emit log(componentName, "managing command SET_RFID, rfid " % image.rfid % " not found for image " % image.filename % ". Removimg img.", "ERROR");
        queueData->dequeue();
    }
}

/*!
 * \fn SendModule::manageTestCrop
 *
 * The slot SendModule::manageTestCrop manage to perform a crop on the image and put it back in the queue for a normal task.
 *
 * \param data string values for the crop.
 */
void SendModule::manageTestCrop(QString data)
{
    // after this step, the image is sent back in queue with normal parameters
    ConnectionImage image = queueData->dequeue();
    emit externalCropImage(data, image.filename, image.userId);
    emit log (componentName, "manage test crop.", "INFO ");
    emit internalTaskStart();
}

/*!
 * \fn SendModule::isConnectionAvailable
 * \return true if the connection with the server on the port 62001 is available, otherwise false.
 */
bool SendModule::isConnectionAvailable()
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP stream socket

    if ((status = getaddrinfo(SERVER_IP, "62001", &hints, &servinfo)) == 0){
        log(componentName, "Test connection successfull.", "INFO ");
        return true;
    } else {
        log(componentName, "Test connection failed! " % QString(gai_strerror(status)) % " (" % QString::number(status) % ".", "ERROR");
        emit connectionErrorSender(QString(gai_strerror(status)), status);
        return false;
    }
}
