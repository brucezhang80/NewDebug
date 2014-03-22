#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T11:57:58
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
    server.cpp \
    server_mainwindow.cpp

HEADERS  += \
    server.h \
    constance.h \
    server_mainwindow.h

FORMS    += \
    server_mainwindow.ui
