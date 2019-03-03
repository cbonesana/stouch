#ifndef RASPBERRYPI_H
#define RASPBERRYPI_H

#include <QObject>
#include <QQueue>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QStringBuilder>
#include <QRegularExpression>
#include <QTimer>

#include <sstream>
#include <fstream>
#include <vector>

#include "Signals/SignalsModule.h"
#include "Signals/SignalsHandlers.h"
#include "Camera/CameraModule.h"
#include "Connection/ConnectionImage.h"
#include "Connection/HTTPManager.h"
#include "Connection/HTTPResponse.h"
#include "Connection/SocketModule.h"
#include "Serial/SerialModule.h"
#include "Log/LogModule.h"
#include "RFID/RFIDModule.h"
#include "RFID/RFIDInventoryModule.h"
#include "Sound/SoundModule.h"
#include "config.h"
#include "Module.h"


using namespace std;

enum ErrorType {
    UNKNOWN,
    NO_RFID,
    RFID_EXCEPTION,
    OPEN_PHOTO_FAIL,
    CONNECTION_ERROR
};

Q_DECLARE_METATYPE(ErrorType)

/*!
 * \brief The RaspberryPi class
 */
class RaspberryPi : public Module
{
    Q_OBJECT

public:
    RaspberryPi(QString config, QQueue<ConnectionImage> *queue, CameraModule *cameraModule, SignalsModule *signalsModule, RFIDModule *rfidModule, RFIDInventoryModule *rfidInventoryModule, QObject *parent = 0);
    virtual ~RaspberryPi();

    void init(SoundModule *soundModule);
    static RaspberryPi *create(QString config,QQueue<ConnectionImage> *queue, CameraModule *cameraModule, SignalsModule *signalsModule, SoundModule *soundModule, RFIDModule *rfidModule, RFIDInventoryModule *rfidInventoryModule, QObject *parent = 0);

    void start();

public slots:
    void RFIDfound(QString rfid);
    void stopTask();
    void startTask();

    void manageBackScan();
    void setCheckUserDet(QString id, QString mac);
    void manageRFIDFound(QString id, QString error);
    void manageCropImage(QString data);
    void manageExternalCropImage(QString data, QString filename, QString userId);

    void firstPhoto();
    void goOfflineMode();
    void goOnlineMode();

signals:
    void contactHttpServer(QString host, QString idUser, QString comando, QString mac, QString param, QString risposta);

    void internalError(ErrorType error);
    void internalTakePhoto();
    void internalFirstPhoto();

    void internalTakePhotoOffline(QString rfid);

    void playSound(QString sound, int r=1);
    void playSound(QString sound1, QString sound2, QString sound3);

    void enableAcquisition();
    void disableAcquisition();

    void startSendTask();

private:
    QString componentName;

    QString param;
    QString macAddress;
    QString ipAddress;
    QString deviceId;

    QQueue<ConnectionImage>   *imageQueue;

    CameraModule        *camera;
    HTTPManager         *httpManager;
    RFIDModule          *rfidManager;
    RFIDInventoryModule *RFIDInventory;
    SignalsModule       *signalsManager;

    QString lastUserId;
    QString lastImage;

    bool enableStartTask;
    bool onlineMode;

    void contactServer();
    QString createFilename(time_t currentTime);

    void checkUserDet();
    void getCrop();
    void restart();

    void setRFID(QString rfidCode);
    void sendImageToServer(QString photoName, QString userId, QString command);
    QString makePhoto();

private slots:
    void buttonRequest();
    void notFoundEnableAgain();

    void manageError(ErrorType error);
    void manageConnectionError(QString error);
    void manageSocketFinished();

    void takePhoto();
    void takePhotoOffline(QString rfidCode);
    void setCrop(QString data);
};

#endif // RASPBERRYPI_H
