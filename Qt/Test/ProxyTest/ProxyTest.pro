#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T13:10:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProxyTest
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ProgressBar.cpp \
    Delegate.cpp \
    Protocol.cpp

HEADERS  += MainWindow.h \
    ProgressBar.h \
    Delegate.h \
    Protocol.h

FORMS    += MainWindow.ui
