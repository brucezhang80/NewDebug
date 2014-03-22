#-------------------------------------------------
#
# Project created by QtCreator 2013-12-06T15:02:07
#
#-------------------------------------------------

QT       += core gui network webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SecKiller
TEMPLATE = app
CONFIG +=  openssl

SOURCES += main.cpp\
        MainWindow.cpp \
    XiaoMi.cpp \
    TaoBao.cpp \
    NetWorkLogin.cpp \
    NetworkCookieJar.cpp \
    TaoBaoWidget.cpp \
    XiaoMiWidget.cpp

HEADERS  += MainWindow.h \
    XiaoMi.h \
    TaoBao.h \
    NetWorkLogin.h \
    NetworkCookieJar.h \
    TaoBaoWidget.h \
    XiaoMiWidget.h

FORMS    += MainWindow.ui
