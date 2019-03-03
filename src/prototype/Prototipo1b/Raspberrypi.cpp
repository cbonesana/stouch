#include "Raspberrypi.h"

/*!
 * \brief RaspberryPi::RaspberryPi
 * \param signalsModule
 * \param serialModule
 * \param logModule
 * \param parent
 */
RaspberryPi::RaspberryPi(
        QString config,
        QQueue<ConnectionImage> *queue,
        CameraModule *cameraModule,
        SignalsModule *signalsModule,
        RFIDModule *rfidModule,
        RFIDInventoryModule *rfidInventoryModule,
        QObject *parent) : Module(config, "Raspberry Pi", parent)
{
    param           = "raspberrypi";
    deviceId        = "";
    macAddress      = "";
    lastUserId      = "";
    imageQueue      = queue;
    enableStartTask = true;

    rfidManager     = rfidModule;
    RFIDInventory   = rfidInventoryModule;

    camera          = cameraModule;
    signalsManager  = signalsModule;

    onlineMode      = true;
}

/*!
 * \brief RaspberryPi::~RaspberryPi
 */
RaspberryPi::~RaspberryPi(){
    signalsManager->stop();
}

void RaspberryPi::init(SoundModule *soundModule)
{
    qRegisterMetaType<ErrorType>("ErrorType");

    connect(camera,         SIGNAL(log(QString,QString,QString)), this, SIGNAL(log(QString,QString,QString)));
    connect(signalsManager, SIGNAL(log(QString,QString,QString)), this, SIGNAL(log(QString,QString,QString)));

    if (rfidManager != NULL){
        connect(rfidManager,SIGNAL(log(QString,QString,QString)), this, SIGNAL(log(QString,QString,QString)));
    }

    if (RFIDInventory != NULL){
        connect(RFIDInventory,  SIGNAL(found(QString)),         this,          SLOT(RFIDfound(QString)));
        connect(RFIDInventory,  SIGNAL(newViewImage()),         this,          SLOT(firstPhoto()));
        connect(this,           SIGNAL(enableAcquisition()),    RFIDInventory, SLOT(enableInventory()));
        connect(this,           SIGNAL(disableAcquisition()),   RFIDInventory, SLOT(disableInventory()));
    }

//    connect(this, SIGNAL(enableAcquisition()),      signalsManager, SLOT(enableEmission()));
//    connect(this, SIGNAL(disableAcquisition()),     signalsManager, SLOT(disableEmission()));

    connect(this, SIGNAL(internalTakePhoto()),      this,           SLOT(takePhoto()));
    connect(this, SIGNAL(internalFirstPhoto()),     this,           SLOT(firstPhoto()));
    connect(this, SIGNAL(internalError(ErrorType)), this,           SLOT(manageError(ErrorType)));

    connect(this, SIGNAL(internalTakePhotoOffline(QString)), this,  SLOT(takePhotoOffline(QString)));

    connect(this, SIGNAL(playSound(QString,int)),       soundModule,    SLOT(play(QString,int)));
    connect(this, SIGNAL(playSound(QString,QString,QString)),   soundModule,    SLOT(play(QString,QString,QString)));

    emit disableAcquisition();
}

RaspberryPi *RaspberryPi::create(
        QString config,
        QQueue<ConnectionImage> *queue,
        CameraModule *cameraModule,
        SignalsModule *signalsModule,
        SoundModule *soundModule,
        RFIDModule *rfidModule,
        RFIDInventoryModule *rfidInventoryModule,
        QObject *parent)
{
    RaspberryPi *raspberryPi = new RaspberryPi(config, queue, cameraModule, signalsModule, rfidModule, rfidInventoryModule, parent);
    raspberryPi->init(soundModule);
    return raspberryPi;
}

void RaspberryPi::start()
{
    emit log(componentName,"Setup initialization completed. Acquisition disabled until setup completing.","SETUP");
}

/*!
 * \brief RaspberryPi::checkUserDet
 * Check for a registered device with this macAddress.
 */
void RaspberryPi::checkUserDet()
{
    emit log(componentName, "Socket executing command: CHECK_USER_DET.", "INFO");
    emit contactHttpServer(
                fileParser->parseFor("server_name", SERVER_NAME) % "?",
                macAddress,
                "CHECK_USER_DET",
                macAddress,
                "STOUCH",
                "checkUserDet"
                );
}

/*!
 * \brief RaspberryPi::buttonRequest
 * Start a RFID inventory after a button event.
 */
void RaspberryPi::buttonRequest()
{
    emit log(componentName,"Button pressed.","INFO ");
    emit disableAcquisition();

    unsigned char source[9] = {0x53, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x5F, 0x30, 0x00};

    RFIDMessage *message;
    try {
        message = rfidManager->inventory(source,9);
        vector<RFID>* founds = message->getRFIDs();

        if (founds->empty()){
            emit log(componentName, "No RFID found.", "ERROR");
            emit internalError(NO_RFID);
            return;
        }

        QString str = QString(RFIDModule::parseRFID(founds->front()));

        emit log(componentName, "Found RFID: " % str % ".", "INFO ");
        setRFID(str);

    } catch (exception& e){
        emit log(componentName, "Exception in RFID inventory function: " % QString(e.what()) % "!", "ERROR");
        emit internalError(RFID_EXCEPTION);
        return;
    }
}

void RaspberryPi::notFoundEnableAgain()
{
//    emit playSound(SoundModule::playError);
    emit enableAcquisition();
}

/*!
 * \brief RaspberryPi::RFIDfound
 * \param rfid
 *
 * When a new RFID is found by the RFIDInventoryModule, this slot is called.
 */
void RaspberryPi::RFIDfound(QString rfid)
{
    emit disableAcquisition();
    emit playSound(SoundModule::playButtonEvent);
    emit log(componentName,"Found RFID: " % rfid % ", acquisition disabled.","INFO ");
    setRFID(rfid);
}

/*!
 * \brief RaspberryPi::setRFID
 * \param rfidCode
 * Register a new RFID.
 */
void RaspberryPi::setRFID(QString rfidCode)
{
    if (onlineMode){
        emit log(componentName, "HTTP executing command: SET_RFID (" % deviceId % "," % rfidCode % ").", "INFO ");
        emit contactHttpServer(
                    fileParser->parseFor("server_name", SERVER_NAME) % "?",
                    deviceId,
                    "SET_RFID",
                    rfidCode,
                    "",
                    "setRFID"
                    );
    } else {
        emit log(componentName, "internal take photo offline with rfid: " % rfidCode % ".", "OFF  ");
        emit internalTakePhotoOffline(rfidCode);
    }
}

/*!
 * \brief RaspberryPi::getCropSize
 */
void RaspberryPi::getCrop()
{
    emit log(componentName, "Socket executing command: GET_CROP.", "INFO ");
    emit contactHttpServer(
                fileParser->parseFor("server_name", SERVER_NAME) % "?",
                deviceId,
                "GET_CROP",
                deviceId,
                "",
                "getCrop"
                );
}

void RaspberryPi::restart()
{
    emit log(componentName, "Try a restart in 10 seconds.", "ERROR");
    QTimer::singleShot(10000,this, SLOT(firstPhoto()));
}

/*!
 * \brief RaspberryPi::sendImageToServer
 * \param photoName
 * \param userId 9999 if not exist
 * \param command VIEWIMG or LOADIMG
 */
void RaspberryPi::sendImageToServer(QString photoName, QString userId, QString command)
{
    emit log(componentName, "Send image to module: " % photoName % " user: " % userId % " (" % command % ").", "INFO ");

    ConnectionImage image;
    image.deviceId   = deviceId;
    image.command    = command;
    image.macAddress = macAddress;
    image.filename   = photoName;
    image.userId     = userId;
    image.rfid       = "";

    imageQueue->enqueue(image);
    emit enableAcquisition();

    if (enableStartTask){
        emit log(componentName, "New data in queue.", "INFO ");
        emit playSound(SoundModule::playWait);
        emit startSendTask();
    }
}

QString RaspberryPi::makePhoto()
{
    emit playSound(SoundModule::playVoicePrepare, SoundModule::playVoiceCountdown, SoundModule::playButtonEvent);

    lastImage = camera->takePhoto(fileParser->parseFor("take_photo",TAKE_PHOTO).toInt());
    QString dirFilename = fileParser->parseFor("img_dir", IMG_DIR) % lastImage;

    emit log(componentName, "New photo: " % dirFilename % ".", "INFO ");

    return lastImage;
}

void RaspberryPi::manageError(ErrorType error)
{
    // TODO
    emit playSound(SoundModule::playError);
    switch (error) {
    case UNKNOWN:
        emit log(componentName, "Unknown error!", "ERROR");
        restart();
        break;
    case CONNECTION_ERROR:
        emit log(componentName, "Error with the connections.", "ERROR");
        restart();
        break;
    case NO_RFID:
        emit log(componentName, "No RFID found!", "ERROR");
        emit enableAcquisition();
        break;
    case RFID_EXCEPTION:
        emit log(componentName, "RFID exception!", "ERROR");
        emit enableAcquisition();
        break;
    case OPEN_PHOTO_FAIL:
        emit log(componentName, "Can't open photo!", "ERROR");
        emit enableAcquisition();
        break;
    default:
        break;
    }
}

void RaspberryPi::manageConnectionError(QString error)
{
    emit log(componentName, "Connection error: " % error % ".", "INFO ");
    emit internalError(CONNECTION_ERROR);
}

void RaspberryPi::takePhoto()
{
    makePhoto();
    getCrop();
}

void RaspberryPi::firstPhoto()
{
    lastImage = camera->takePhoto(1000);

    QString dirFilename = fileParser->parseFor("img_dir", IMG_DIR) % lastImage;

    emit playSound(SoundModule::playVoiceActive);
    emit log(componentName, "Initial photo: " % dirFilename, "SETUP");

    sendImageToServer(dirFilename, "9999", "VIEWIMG");
}

void RaspberryPi::takePhotoOffline(QString rfidCode)
{
    QString filename = makePhoto();

    ConnectionImage image;
    image.deviceId   = deviceId;
    image.command    = "LOADIMG";
    image.macAddress = macAddress;
    image.filename   = filename;
    image.userId     = "";
    image.rfid       = rfidCode;

    imageQueue->enqueue(image);
    emit enableAcquisition();
    emit log(componentName, "New image queued and waiting for connection (" % filename % ", " % rfidCode % ").");
    emit playSound(SoundModule::playWait);
}

void RaspberryPi::setCrop(QString data)
{
    QStringList list = data.split(QRegularExpression("\\b"));

    camera->setX1(list.at(3));
    camera->setY1(list.at(5));
    camera->setX2(list.at(7));
    camera->setY2(list.at(9));

    emit log(componentName, "Crop parameters changed to ("
             % camera->getX1() % ";" % camera->getY1() % ") ("
             % camera->getX2() % ";" % camera->getY2() % ").", "INFO ");
}

void RaspberryPi::stopTask()
{
    if (!enableStartTask){
        emit log(componentName, "Start sending data process. New signals disabled.", "INFO ");
        enableStartTask = false;
    }
}

void RaspberryPi::startTask()
{
    emit log(componentName, "Queue is empty, all data sent. New signals enabled.", "INFO ");
    //emit playSound(SoundModule::playSend);

    qDebug() << "image queue: " << imageQueue;
    if (!imageQueue->isEmpty()){
        emit log(componentName, "New data in queue, " % QString::number(imageQueue->size()) % " element(s).", "INFO ");
        emit startTask();
    } else {
        enableStartTask = true;
    }
}

void RaspberryPi::manageBackScan()
{
    emit log(componentName, "HTTP Module reported to go back scanning.", "INFO ");
    emit playSound(SoundModule::playError);
    emit enableAcquisition();
}

void RaspberryPi::setCheckUserDet(QString id, QString mac)
{
    deviceId   = id;
    macAddress = mac;
    emit log(componentName,"device ID set to " % deviceId % ".","SETUP");
    emit internalFirstPhoto();
}

void RaspberryPi::manageRFIDFound(QString id, QString error)
{
    emit log(componentName, "setRFID, errore = " % error % " risposta = " % id % ".", "INFO ");
    if (error == "OK"){
        emit log(componentName,"setRFID, idUser: " % id % ".","LOG  ");
        lastUserId = id;

        // Easter Egg ;)
        if (id == "15387"){
            emit playSound(SoundModule::playCreativo);
        }

        emit internalTakePhoto();
    } else {
        lastUserId = "";
        emit playSound(SoundModule::playVoiceErrBracc);
        emit log(componentName, "setRFID, idUser not available.", "LOG  ");

        QString timeout = fileParser->parseFor("rfid_not_found_time", RFID_NOT_FOUND_TIME);
        QTimer::singleShot(timeout.toInt(),this, SLOT(notFoundEnableAgain()));
    }
}

void RaspberryPi::manageCropImage(QString data)
{
    setCrop(data);
    emit log(componentName, "Operation complete, start crop.","INFO ");

    QString dirFilename = fileParser->parseFor("crop_dir", CROP_DIR) % lastImage;

    camera->cropImage(lastImage);
    sendImageToServer(dirFilename, lastUserId, "LOADIMG");
}

void RaspberryPi::manageExternalCropImage(QString data, QString filename, QString userId)
{
    setCrop(data);
    emit log(componentName, "Operation complete, start external crop for userid " % userId % ", filename " % filename % ".","INFO ");

    QString dirFilename = fileParser->parseFor("crop_dir", CROP_DIR) % filename;
    lastUserId = userId;

    camera->cropImage(filename);
    sendImageToServer(dirFilename, userId, "LOADIMG");
}

void RaspberryPi::goOfflineMode()
{
    onlineMode = false;
    emit log(componentName, "I'm in offline mode now.", "OFF  ");
}

void RaspberryPi::goOnlineMode()
{
    onlineMode = true;
    emit log(componentName, "I'm back in online mode now.", "ON   ");
}

void RaspberryPi::manageSocketFinished()
{
    emit playSound(SoundModule::playSend);
    emit enableAcquisition();
}
