#-------------------------------------------------
#
# Project created by QtCreator 2013-06-24T14:14:18
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QT_touch
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    serialthread.cpp \
    signalpool.cpp

HEADERS += \
    serialthread.h \
    signalpool.h
