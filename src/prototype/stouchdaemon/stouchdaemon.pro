#-------------------------------------------------
#
# Project created by QtCreator 2013-07-28T15:25:16
#
#-------------------------------------------------

TARGET = stouchdaemon
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

INCLUDEPATH += /mnt/rasp-pi-rootfs/usr/local/include
LIBS += -L/mnt/rasp-pi-rootfs/usr/local/lib -lwiringPi

SOURCES += main.cpp

#target.path = /home/pi/Development/daemon/
target.path = /opt/stouch/
INSTALLS += target
