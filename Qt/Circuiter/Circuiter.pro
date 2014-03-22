#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T15:19:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Circuiter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dataclient.cpp \
    qtfilecopier.cpp \
    jobsenderview.cpp \
    listwidget.cpp \
    jobhistoryview.cpp \
    personmanagerview.cpp \
    personwidget.cpp \
    tcpserver.cpp \
    connection.cpp \
    personclient.cpp \
    peerbroadcast.cpp

HEADERS  += mainwindow.h \
    dataclient.h \
    qtfilecopier.h \
    jobsenderview.h \
    listwidget.h \
    jobhistoryview.h \
    personManagerView.h \
    personwidget.h \
    tcpserver.h \
    connection.h \
    personclient.h \
    peerbroadcast.h

FORMS    += mainwindow.ui \
    personwidget.ui

RESOURCES += \
    res.qrc
