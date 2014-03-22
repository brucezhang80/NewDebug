#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T10:11:33
#
#-------------------------------------------------

QT       += network

win32:CONFIG(release, debug|release):TARGET = Connect
else:win32:CONFIG(debug, debug|release):TARGET = Connectd

TEMPLATE = lib

DEFINES += CONNECT_LIBRARY

SOURCES += \
    server.cpp \
    connection.cpp \
    client.cpp

HEADERS +=\
        Connect_global.h \
    server.h \
    connection.h \
    client.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE707CBAF
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Connect.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

DLLDESTDIR = $$PWD/../common/bin
DESTDIR = $$PWD/../common/lib

OTHER_FILES += \
    cp.bat
