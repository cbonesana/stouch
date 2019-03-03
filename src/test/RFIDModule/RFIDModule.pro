#-------------------------------------------------
#
# Project created by QtCreator 2013-07-12T15:59:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = RFIDModule
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../Prototipo1b/Serial/SerialModule.cpp \
    ../Prototipo1b/Log/LogModule.cpp \
    ../Prototipo1b/RFID/RFIDMessage.cpp \
    ../Prototipo1b/RFID/RFIDModule.cpp \
    ../Prototipo1b/RFID/RFIDException.cpp \
    ../Prototipo1b/Module.cpp

HEADERS += \
    ../Prototipo1b/Serial/SerialModule.h \
    ../Prototipo1b/Log/LogModule.h \
    ../Prototipo1b/RFID/RFIDMessage.h \
    ../Prototipo1b/RFID/RFIDCommands.h \
    ../Prototipo1b/RFID/RFIDModule.h \
    ../Prototipo1b/RFID/RFIDAttributeTypes.h \
    ../Prototipo1b/RFID/RFIDCommandsCodes.h \
    ../Prototipo1b/RFID/RFIDException.h \
    ../Prototipo1b/Module.h

target.path = /home/pi/Development
INSTALLS += target
