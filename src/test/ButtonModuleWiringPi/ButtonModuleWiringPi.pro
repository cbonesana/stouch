#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T13:17:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ButtonModuleWiringPi
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /mnt/rasp-pi-rootfs/usr/local/include
LIBS += -L/mnt/rasp-pi-rootfs/usr/local/lib -lwiringPi

SOURCES += main.cpp \
    ../Prototipo1b/Module.cpp \
    ../Prototipo1b/Buttons/ButtonModule.cpp \
    ../Prototipo1b/Buttons/ButtonException.cpp \
    ../Prototipo1b/Buttons/ButtonInit.cpp \
    TestWiringPi.cpp

HEADERS += \
    ../Prototipo1b/Module.h \
    ../Prototipo1b/Buttons/ButtonModule.h \
    ../Prototipo1b/Buttons/ButtonException.h \
    ../Prototipo1b/Buttons/ButtonInit.h \
    TestWiringPi.h

target.path = /home/pi/Development
INSTALLS += target
