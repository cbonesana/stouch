#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QString>
#include <QStringBuilder>

#include <QTime>
#include <QDate>

#include "../Module.h"
#include "../config.h"

#include <stdio.h>

/*!
 *  \class CameraModule
 *  \brief The CameraModule class is used to manage image acquisition and manipulation.
 *
 *  The CameraModule class is the only Module who can take, save and manipulate photos.
 *  It use an external program build with OpenCV to performa a simple crop based on local parameters.
 *
 *  The object of this class is registered in log with the name "Camera Module", where no other names are provided.
 */
class CameraModule : public Module
{
    Q_OBJECT
public:
    CameraModule(QString config, QString componentName = "Camera Module", QObject *parent = 0);
    ~CameraModule();

    QString takePhoto(int millisec=5000);
    void takePhoto(QString filename, int millisec=5000);
    void cropImage(QString filename);

    void setX1(QString x1){ this->x1 = x1; emit log(componentName, "set x1 to " % x1 % ".", "INFO "); }
    void setY1(QString y1){ this->y1 = y1; emit log(componentName, "set y1 to " % y1 % ".", "INFO "); }
    void setX2(QString x2){ this->x2 = x2; emit log(componentName, "set x2 to " % x2 % ".", "INFO "); }
    void setY2(QString y2){ this->y2 = y2; emit log(componentName, "set y2 to " % y2 % ".", "INFO "); }

    QString getX1(){ return x1; }
    QString getY1(){ return y1; }
    QString getX2(){ return x2; }
    QString getY2(){ return y2; }

signals:
    void photoTaked();
    void error(int errno);

private:
    QString imgDir;         /*! Where the fresh images will be stored. */
    QString cropDir;        /*! Where the cropped images will be stored. */

    QTime lastTimePhoto;    /*! Time of the last photo. */
    QDate lastDatePhoto;    /*! Date of the last photo. */

    QString x1;     /*! First point X parameter used to crop. */
    QString y1;     /*! First point Y parameter used to crop. */
    QString x2;     /*! Second point X parameter used to crop. */
    QString y2;     /*! Second point Y parameter used to crop. */
};

#endif // CAMERA_H
