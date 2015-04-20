#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T17:52:33
#
#-------------------------------------------------

QT       += core gui sql widgets

TARGET = qtcrezoo
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    QCheckboxFilterModel.cpp \
    QColumnByName.cpp \
    QCSString.cpp \
    QDBQueryModel.cpp \
    QDict.cpp \
    QFileName.cpp \
    QMirrorModel.cpp \
    QSelectProxy.cpp \
    QShadowDataModel.cpp \
    QTextFile.cpp \
    model_tools.cpp

HEADERS += \
    QCheckboxFilterModel.h \
    QColumnByName.h \
    QCSString.h \
    QDBQueryModel.h \
    QDict.h \
    QFileName.h \
    QMirrorModel.h \
    QSelectProxy.h \
    QShadowDataModel.h \
    QTextFile.h \
    model_tools.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
