#include "CameraModule.h"

/*!
 *  \fn CameraModule::CameraModule
 *
 *  Constructs the module based on \a imageDir, \a croppingDir and a mandatory \a parent.
 *
 *  \param config path to the external configuration file.
 *  \param componentName this parametr indicates the name of the component.
 *  \param parent
 */
CameraModule::CameraModule(QString config, QString componentName, QObject *parent) : Module (config, componentName, parent)
{
    imgDir = fileParser->parseFor("img_dir", IMG_DIR);
    cropDir = fileParser->parseFor("crop_dir", CROP_DIR);
    x1 = "0";
    y1 = "0";
    x2 = "0";
    y2 = "0";
}

/*!
 *  \fn CameraModule::~CameraModule
 *
 *  Destructs the module.
 */
CameraModule::~CameraModule()
{}

/*!
 * \fn CameraModule::photoTaked
 *
 * The signal CameraModule::photoTaked is emitted when a photo is sucessfully taken.
 */

/*!
 * \fn CameraModule::error
 * Not used.
 */

/*!
 * \brief The method CameraModule::takePhoto takes a photo with a wait time of \a millisec milliseconds and name it with the current time.
 *
 * \param millisec time to wait before take the photo in millisec. Avoid a value less than 1000 millisecond
 * \return the name of the photo based on the time
 */
QString CameraModule::takePhoto(int millisec)
{
    lastTimePhoto = QTime::currentTime();
    lastDatePhoto = QDate::currentDate();

    QString filename =
            lastDatePhoto.toString("yyyy-MM-dd") % "_" %
            lastTimePhoto.toString("hh-mm-ss") % ".jpg";

    takePhoto(filename, millisec);

    return filename;
}

/*!
 * \brief The CameraModule::takePhoto takes a photo with a given \a millisec time and a \a filename for the photo.
 *
 * \param filename name of the file
 * \param millisec time to wait
 */
void CameraModule::takePhoto(QString filename, int millisec)
{
    QString command = "raspistill -t " % QString::number(millisec) % " -o " % imgDir % filename;

    system(qPrintable(command));

    emit log(componentName, "New photo acquired: " % filename % " .", "INFO ");
    emit photoTaked();
}

/*!
 * \brief The CameraModule::cropImage method crops the image \a filename and put the cropped version in the \a cropDir directory.
 *
 * \param filename the name of the image to crop.
 */
void CameraModule::cropImage(QString filename)
{
    QString command = "./cropper " % imgDir % filename % " " % cropDir % filename % " " % x1 % " " % y1 % " " % x2 % " " % y2;
    system(qPrintable(command));

    emit log(componentName, "Cropped image " % filename % ".", "INFO ");
}
