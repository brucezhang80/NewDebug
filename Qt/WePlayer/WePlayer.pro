#-------------------------------------------------
#
# Project created by QtCreator 2013-12-12T17:13:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WePlayer
TEMPLATE = app


SOURCES += main.cpp\
        WePlayer.cpp \
    MusicItemWidget.cpp \
    MusicListWidget.cpp

HEADERS  += WePlayer.h \
    MusicItemWidget.h \
    MusicListWidget.h

FORMS    += WePlayer.ui \
    MusicItemWidget.ui
