#-------------------------------------------------
#
# Project created by QtCreator 2013-11-01T14:22:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphViewer
TEMPLATE = app
#TEMPLATE = lib

SOURCES += main.cpp\
    PixmapItem.cpp \
    PixmapView.cpp \
    ThumbnailView.cpp \
    ThumbnailItem.cpp \
    ThumbnailBar.cpp \
    GraphViewer.cpp \
    qwin/qwinwidget.cpp \
    qwin/qwinhost.cpp \
    qwin/qmfcapp.cpp

HEADERS  += \
    PixmapItem.h \
    PixmapView.h \
    ThumbnailView.h \
    ThumbnailItem.h \
    ThumbnailBar.h \
    GraphViewer.h \
    qwin/qwinwidget.h \
    qwin/qwinhost.h \
    qwin/qmfcapp.h

FORMS    += \
    GraphViewer.ui

LIBS += -luser32 \

DEPENDPATH += windows.h
DEFINES -= UNICODE
win32:QMAKE_CXXFLAGS += -wd4100 -wd4819 -wd4099
