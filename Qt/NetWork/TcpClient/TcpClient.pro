#-------------------------------------------------
#
# Project created by QtCreator 2013-07-21T05:57:13
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientsocket.cpp

HEADERS  += mainwindow.h \
    clientsocket.h

FORMS    += mainwindow.ui
