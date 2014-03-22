#-------------------------------------------------
#
# Project created by QtCreator 2012-12-15T16:59:39
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PicGet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pictureview.cpp \
    picproject.cpp \
    projectview.cpp \
    newprojectdlg.cpp \
    treeitem.cpp \
    treemodel.cpp \
    pictureclient.cpp \
    filemanthread.cpp \
    uniqueapp.cpp

HEADERS  += mainwindow.h \
    pictureview.h \
    picproject.h \
    projectview.h \
    newprojectdlg.h \
    treeitem.h \
    treemodel.h \
    pictureclient.h \
    filemanthread.h \
    uniqueapp.h

OTHER_FILES += \
    icons/stop.png \
    icons/player_stop.png \
    icons/player_play.png \
    icons/player_pause.png \
    icons/exit.png \
    icons/edit_remove.png \
    icons/edit_add.png \
    icons/bottom.png \
    icons/1uparrow.png \
    icons/1downarrow.png

RESOURCES += \
    icons.qrc
