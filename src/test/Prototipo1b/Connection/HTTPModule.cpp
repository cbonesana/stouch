#include "HTTPModule.h"

/*!
 * \fn HTTPModule::HTTPModule
 *
 * Constructs the module but it doesn't initialize the attributes.
 *
 * \param config path to the external configuration file.
 * \param debug if set to TRUE, the module will print some informations on the stdout.
 */
HTTPModule::HTTPModule(QString config, bool debug) : Module(config, "HTTPModule")
{
    httpManager = NULL;
    onlineMode  = true;
    this->debug = debug;
}

/*!
 * \fn HTTPModule::~HTTPModule
 *
 * Destructs the module.
 */
HTTPModule::~HTTPModule()
{
    delete(httpManager);
}

/*!
 * \fn HTTPModule::sigBackScan
 *
 * The signal HTTPModule::sigBackScan is used to notify to restart the inventory sequence because there was a connection error and data was lost.
 */

/*!
 * \fn HTTPModule::sigCheckUserDet
 *
 * The signal HTTPModule::sigCheckUserDet is used on setup sequence to set the id and mac address of the device.
 *
 * \param id id of the device.
 * \param mac mac address of the device.
 */

/*!
 * \fn HTTPModule::sigSetRFID
 *
 * The signal HTTPModule::sigSetRFID is used to notify the result from the server on a specified RFID.
 *
 * \param id id of the user.
 * \param error status of the response. If OK, there is a user connected with the rfid, otherwise no.
 */

/*!
 * \fn HTTPModule::sigTestRFID
 *
 * The signal HTTPModule::sigTestRFID is used after offline mode to test notify the result from the server on a specified RFID.
 *
 * \param id id of the user.
 * \param error status of the response. If OK, there is a user connected with the rfid, otherwise no.
 */

/*!
 * \fn HTTPModule::sigGetCrop
 *
 * The signal HTTPModule::sigGetCrop is used to notify the crop parameters from the server.
 *
 * \param data string that contains the four crop values.
 */

/*!
 * \fn HTTPModule::sigTestCrop
 *
 * The signal HTTPModule::sigTestCrop is used after offline mode to test the crop parameters from the server.
 *
 * \param data string that contains the four crop values.
 */

/*!
 * \fn HTTPModule::connectionErrorHttp
 *
 * The signal HTTPModule::connectionErrorHttp is used to notify an error on the connection to the server.
 *
 * \param error number that defines the connection error.
 */

/*!
 * \fn HTTPModule::getHttpManager
 *
 * The method HTTPModule::getHttpManager use a singleton pattern to access the local HTTPModule.
 * On first request, this method create a HTTPModule objectm set the connection and complete the system setup with the device id and mac address.
 *
 * \return the current HTTPManager available.
 */
HTTPManager *HTTPModule::getHttpManager()
{
    if (httpManager == NULL){
        httpManager = new HTTPManager();

        connect(httpManager, SIGNAL(connectionError(QString)),                 this, SLOT(manageConnectionError(QString)));
        connect(httpManager, SIGNAL(responseAvailable(QString,HTTPResponse*)), this, SLOT(manageResponses(QString,HTTPResponse*)));

        macAddress      = httpManager->getMacAddress();
        ipAddress       = httpManager->getIpAddress();

        emit log(componentName,"MAC Address set to " % macAddress % ".","SETUP");
        emit log(componentName,"IP Address set to " % ipAddress % ".","SETUP");

        httpManager->contactServer(
                    fileParser->parseFor("server_name", SERVER_NAME) % "?",
                    macAddress,
                    "CHECK_USER_DET",
                    macAddress,
                    "STOUCH",
                    "checkUserDet"
                    );
    }
    return httpManager;
}

/*!
 * \fn HTTPModule::logOnServer
 *
 * The slot HTTPModule::logOnServer writes a string on the server. It is used to log critical information.
 *
 * \param message string to log on the server.
 */
void HTTPModule::logOnServer(QString message)
{
    getHttpManager()->contactServer(
                fileParser->parseFor("server_name", SERVER_NAME) % "?",
                deviceId,
                "DEBUG",
                "",
                message,
                "logOnServer"
                );
    if (debug){
        qDebug() << qPrintable(message);
    }
}

/*!
 * \fn HTTPModule::manageConnectionError
 *
 * The slot HTTPModule::manageConnectionError manages and notify the connection error.
 *
 * \param error number of the error.
 */
void HTTPModule::manageConnectionError(QString error)
{
    if (onlineMode){
        emit log(componentName, "Connection error: " % error % ".", "ERROR");
        emit connectionErrorHttp(error);
        emit sigBackScan();
    }
}

/*!
 * \fn HTTPModule::manageResponses
 *
 * The slot HTTPModule::manageResponses manages the replies from the serve via an HTTPManager object and sent the right signal to the right module.
 *
 * \param item name of the JSON object, used to identify a specified request.
 * \param response object that contains parameters received from the server.
 */
void HTTPModule::manageResponses(QString item, HTTPResponse *response)
{
    if (item == "checkUserDet") {
        deviceId = response->get("risposta");

        emit sigCheckUserDet(deviceId, macAddress);
        emit log(componentName, "checkUserDet, rispsota: " % deviceId % ".", "INFO ");

    } else if (item == "setRFID") {
        QString risposta = response->get("risposta");
        QString errore   = response->get("errore");

        emit sigSetRFID(risposta, errore);
        emit log(componentName, "setRFID, risposta: " % risposta % ", errore: " % errore % ".", "INFO ");

    } else if (item == "testRFID"){
        QString risposta = response->get("risposta");
        QString errore   = response->get("errore");

        emit sigTestRFID(risposta, errore);
        emit log(componentName, "testRFID, risposta: " % risposta % ", errore: " % errore % ".", "INFO ");

    } else if (item == "getCrop") {
        QString risposta = response->get("risposta");

        emit sigGetCrop(risposta);
        emit log(componentName, "getCrop, risposta: " % risposta % ".", "INFO ");
    } else if (item == "testCrop") {
        QString risposta = response->get("risposta");

        emit sigTestCrop(risposta);
        emit log(componentName, "testCrop, risposta: " % risposta % ".", "INFO ");
    }
}

/*!
 * \fn HTTPModule::contactServer
 *
 * The slot HTTPModule::contactServer use the parameters to contact the server with specified parameters. It works only in online mode.
 *
 * \param host a valid name of the web server
 * \param idUser device id
 * \param command command to execute on the server
 * \param mac mac address or special id
 * \param param various parameters
 * \param risposta name of the JSON object in the answear
 */
void HTTPModule::contactServer(QString host, QString idUser, QString comando, QString mac, QString param, QString risposta)
{
    if (onlineMode){
        getHttpManager()->contactServer(host, idUser, comando, mac, param, risposta);
    }
}

/*!
 * \fn HTTPModule::goOfflineMode
 *
 * The slot HTTPModule::goOffline switches to offline mode.
 */
void HTTPModule::goOfflineMode()
{
    onlineMode = false;
    emit log(componentName, "I'm in offline mode now.", "OFF  ");
}

/*!
 * \fn HTTPModule::goOnlineMode
 *
 * The slot HTTPModule::goOnline switches to online mode.
 */
void HTTPModule::goOnlineMode()
{
    onlineMode = true;
    emit log(componentName, "I'm back in online mode now.", "ON   ");
}
