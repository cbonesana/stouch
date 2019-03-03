#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T15:54:44
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SoundManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../Prototipo1b/Sound/SoundModule.cpp \
    ../Prototipo1b/Module.cpp \
    ../Prototipo1b/FileParser.cpp \
    TestSOund.cpp

HEADERS += \
    ../Prototipo1b/Sound/SoundModule.h \
    ../Prototipo1b/Module.h \
    ../Prototipo1b/FileParser.h \
    TestSOund.h

target.path = /home/pi/Development/Prototipo1b
INSTALLS += target
