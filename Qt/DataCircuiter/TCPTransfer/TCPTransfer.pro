#-------------------------------------------------
#
# Project created by QtCreator 2013-04-01T16:07:22
#
#-------------------------------------------------

QT       += core gui network

TARGET = TCPTransfer
TEMPLATE = app


SOURCES += main.cpp\
        tcptransfer.cpp \
    friendsview.cpp \
    filesview.cpp \
    historyupload.cpp \
    qtfilecopier.cpp \
    dataclient.cpp \
    server.cpp \
    peermanager.cpp \
    connection.cpp \
    client.cpp \
    privateclient.cpp \
    privatechatdlg.cpp

HEADERS  += tcptransfer.h \
    friendsview.h \
    filesview.h \
    historyupload.h \
    qtfilecopier.h \
    dataclient.h \
    server.h \
    peermanager.h \
    connection.h \
    client.h \
    privateclient.h \
    privatechatdlg.h

FORMS    += tcptransfer.ui \
    privateclient.ui \
    privatechatdlg.ui

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../common/lib/ -lConnect
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../common/lib/ -lConnectd
#else:unix: LIBS += -L$$PWD/../common/lib/ -lConnect

#INCLUDEPATH += $$PWD/../common/include
#DEPENDPATH += $$PWD/../common/include

RESOURCES += \
    res.qrc
