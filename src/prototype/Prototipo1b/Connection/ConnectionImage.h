#ifndef CONNECTIONIMAGE_H
#define CONNECTIONIMAGE_H

#include <QString>

/*!
 * \struct ConnectionImage
 *
 * \brief Used to store image data.
 *
 * The ConnectionImage struct is used to store information about images from the camera.
 * Non every attribute is used. Some attributes, like \a rfid is only used in offline mode.
 */
typedef struct {
    QString deviceId;       /*! Actual device id. */
    QString command;        /*! Command for the server. */
    QString macAddress;     /*! Registered mac address of the device. */
    QString filename;       /*! Name of the photo. */
    QString userId;         /*! Id of the user who took the photo. */
    QString rfid;           /*! RFID tag value of the user who took the photo. */
} ConnectionImage;

#endif // CONNECTIONIMAGE_H
