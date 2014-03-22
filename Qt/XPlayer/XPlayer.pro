#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T09:58:20
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XPlayer
TEMPLATE = app


SOURCES += main.cpp\
        XPlayer.cpp \

HEADERS  += XPlayer.h \
    ThreadManager.h \
    JsonParser.h

include (MainWindowEx/MainWindowEx.pri)
include (MediaPlayWidget/MediaPlayWidget.pri)
include (MediaHintButtonWidget/MediaHintButtonWidget.pri)
include (MediaPlayListWidget/MediaPlayListWidget.pri)
include (MediaAlbumWidget/MediaAlbumWidget.pri)
include (MediaSearchWidget/MediaSearchWidget.pri)
include (MediaProgressBar/MediaProgressBar.pri)
include (StateMachineServer/StateMachineServer.pri)
include (MediaDataManager/MediaDataManager.pri)


RESOURCES += \
    XPlayer.qrc
