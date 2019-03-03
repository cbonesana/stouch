#-------------------------------------------------
#
# Project created by QtCreator 2013-07-01T09:47:13
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Prototipo1b
CONFIG   += console
CONFIG   -= app_bundle


TEMPLATE = app

INCLUDEPATH += /mnt/rasp-pi-rootfs/usr/local/include
LIBS += -L/mnt/rasp-pi-rootfs/usr/local/lib -lwiringPi

SOURCES += main.cpp                     \
    Camera/CameraModule.cpp             \
    Connection/HTTPManager.cpp          \
    Connection/HTTPResponse.cpp         \
    Connection/SocketModule.cpp         \
    Log/LogModule.cpp                   \
    RFID/RFIDException.cpp              \
    RFID/RFIDMessage.cpp                \
    RFID/RFIDModule.cpp                 \
    Serial/SerialModule.cpp             \
    Signals/SignalsException.cpp        \
    Signals/SignalsModule.cpp           \
    Sound/SoundModule.cpp               \
    Module.cpp                          \
    Raspberrypi.cpp                     \
    Signals/SignalsHandlers.cpp \
    RFID/RFIDInventoryModule.cpp \
    Connection/SendModule.cpp \
    Connection/HTTPModule.cpp \
    FileParser.cpp

HEADERS +=                              \
    Camera/CameraModule.h               \
    Connection/HTTPManager.h            \
    Connection/HTTPResponse.h           \
    Connection/SocketModule.h           \
    Log/LogModule.h                     \
    RFID/RFIDCommandsCodes.h            \
    RFID/RFIDException.h                \
    RFID/RFIDMessage.h                  \
    RFID/RFIDModule.h                   \
    RFID/RFIDattributeTypes.h           \
    Serial/SerialModule.h               \
    Signals/SignalsException.h          \
    Signals/SignalsModule.h             \
    Sound/SoundModule.h                 \
    Module.h                            \
    Raspberrypi.h \
    Signals/SignalsHandlers.h \
    Connection/SocketModule.h \
    RFID/RFIDInventoryModule.h \
    config.h \
    Connection/SendModule.h \
    Connection/ConnectionImage.h \
    Connection/HTTPModule.h \
    FileParser.h

target.path = /opt/stouch/
INSTALLS += target
