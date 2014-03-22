#-------------------------------------------------
#
# Project created by QtCreator 2013-07-21T05:56:37
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversocket.cpp

HEADERS  += mainwindow.h \
    serversocket.h

FORMS    += mainwindow.ui
