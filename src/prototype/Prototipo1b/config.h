#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FILE "/opt/stouch/stouch.config"
#define SOUND_CONF  "/opt/stouch/sounds.config"

#define CLIENT_PID_FILE "/opt/stouch/daemon/client.pid"

#define LOG_NAME    "/opt/stouch/log/STOUCH_LOG"
#define SOUND_DIR   "/opt/stouch/sound/"
#define CROP_DIR    "/opt/stouch/crop/"
#define IMG_DIR     "/opt/stouch/img/"

#define PHOTO_NAME  "image.jpg"
#define FIRST_IMG   "sfondo.jpg"

#define SERVER_NAME "http://www.tv-surf.com/3D-Enter/servlet/P13_19_CommandXML"
#define SERVER_IP   "62.2.177.143"
#define SERVER_PORT "62001"

#define RFID_NOT_FOUND_TIME     "3000"  // time in milliseconds
#define INVENTORY_TIME          "1200"  // time in milliseconds
#define SCAN_TO_LOG             "300"   // how many inventory scans before log to serve
#define TAKE_PHOTO              "8000"

#endif // CONFIG_H
