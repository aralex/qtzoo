#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T17:52:33
#
#-------------------------------------------------

QT       -= gui

TARGET = qtcrezoo
TEMPLATE = lib
CONFIG += staticlib

SOURCES += qtcrezoo.cpp

HEADERS += qtcrezoo.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
