#-------------------------------------------------
#
# Project created by QtCreator 2013-09-26T09:46:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageDownload
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    HttpDownload.cpp \
    ParseHtml.cpp

HEADERS  += MainWindow.h \
    HttpDownload.h \
    ParseHtml.h

FORMS    += MainWindow.ui
