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
    model_tools.cpp \
    qCheckboxFilterModel.cpp \
    qCSString.cpp \
    qColumnByName.cpp \
    qDBQueryModel.cpp \
    qDict.cpp \
    qTextFile.cpp \
    qFileName.cpp \
    qMirrorModel.cpp \
    qSelectProxy.cpp \
    qShadowDataModel.cpp \
    qSpyModel.cpp

HEADERS += \
    model_tools.h \
    qCheckboxFilterModel.h \
    qCSString.h \
    qColumnByName.h \
    qDBQueryModel.h \
    qDict.h \
    qTextFile.h \
    qFileName.h \
    qMirrorModel.h \
    qSelectProxy.h \
    qShadowDataModel.h \
    qSpyModel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
