#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T20:10:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_chb_filt_model
TEMPLATE = app


SOURCES += main.cpp\
        testchbfiltmainwindow.cpp

HEADERS  += testchbfiltmainwindow.h

FORMS    += testchbfiltmainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qtcrezoo/release/ -lqtcrezoo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qtcrezoo/debug/ -lqtcrezood
else:unix: LIBS += -L$$OUT_PWD/../qtcrezoo/ -lqtcrezoo

INCLUDEPATH += $$PWD/../qtcrezoo
DEPENDPATH += $$PWD/../qtcrezoo

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtcrezoo/libqtcrezoo.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtcrezoo/libqtcrezood.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtcrezoo/release/qtcrezoo.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtcrezoo/debug/qtcrezood.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qtcrezoo/libqtcrezoo.a

RESOURCES += \
    resources.qrc
