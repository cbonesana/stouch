#-------------------------------------------------
#
# Project created by QtCreator 2013-07-30T15:24:58
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = SocketModule
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../Prototipo1b/Connection/SocketModule.cpp \
    ../Prototipo1b/Log/LogModule.cpp \
    ../Prototipo1b/Module.cpp

HEADERS += \
    ../Prototipo1b/Connection/SocketModule.h \
    ../Prototipo1b/Log/LogModule.h \
    ../Prototipo1b/Module.h
