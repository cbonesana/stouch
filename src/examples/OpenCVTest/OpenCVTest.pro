#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T22:03:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = OpenCVTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /mnt/rasp-pi-rootfs/opt/opencv/include \
/mnt/rasp-pi-rootfs/opt/opencv/include/opencv

LIBS += -L/mnt/rasp-pi-rootfs/opt/opencv/lib \
-L/mnt/rasp-pi-rootfs/opt/opencv/share/OpenCV/3rdparty/lib/ \
-lopencv_contrib -lopencv_stitching -lopencv_nonfree -lopencv_superres -lopencv_ts -lopencv_videostab \
-lopencv_gpu -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_calib3d -lopencv_photo -lopencv_video \
-lopencv_features2d -lopencv_highgui -lIlmImf -llibjasper -llibtiff -llibpng -llibjpeg -lopencv_flann \
-lopencv_imgproc -lopencv_core -lzlib -lrt

SOURCES += main.cpp

target.path = /home/pi/Development
INSTALLS += target
