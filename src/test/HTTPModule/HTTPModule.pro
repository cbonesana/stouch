#-------------------------------------------------
#
# Project created by QtCreator 2013-07-20T14:59:39
#
#-------------------------------------------------

QT       += core network script

QT       -= gui

TARGET = HTTPModule
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    ../Prototipo1b/Connection/HTTPResponse.cpp \
    ../Prototipo1b/Connection/HTTPManager.cpp

HEADERS += \
    ../Prototipo1b/Connection/HTTPResponse.h \
    ../Prototipo1b/Connection/HTTPManager.h
