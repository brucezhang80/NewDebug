#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T16:07:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NiQ
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    friendstablewnd.cpp \
    userwidget.cpp \
    common/server.cpp \
    common/peermanager.cpp \
    common/connection.cpp \
    common/client.cpp \
    privatechatwidget.cpp \
    friendstree.cpp \
    systemtrayicon.cpp \
    traynotifydlg.cpp \
    groupwidget.cpp \
    sendfiledlg.cpp \
    filetransfer.cpp \
    fileclient.cpp

HEADERS  += mainwidget.h \
    friendstablewnd.h \
    userwidget.h \
    common/server.h \
    common/peermanager.h \
    common/connection.h \
    common/client.h \
    privatechatwidget.h \
    friendstree.h \
    systemtrayicon.h \
    traynotifydlg.h \
    groupwidget.h \
    sendfiledlg.h \
    filetransfer.h \
    fileclient.h

FORMS    += mainwidget.ui \
    userwidget.ui \
    privatechatwidget.ui \
    traynotifydlg.ui \
    groupwidget.ui \
    sendfiledlg.ui

RESOURCES += \
    res.qrc
