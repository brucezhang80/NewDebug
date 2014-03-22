#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T15:15:55
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
    client.cpp \
    client_mainwindow.cpp

HEADERS  += \
    client.h \
    constance.h \
    client_mainwindow.h

FORMS    += \
    client_mainwindow.ui
