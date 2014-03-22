#-------------------------------------------------
#
# Project created by QtCreator 2013-07-08T08:59:39
#
#-------------------------------------------------

QT       += core gui qaxserver qaxcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scripts
TEMPLATE = lib
#CONFIG += staticlib

SOURCES += main.cpp\
        scripts_widget.cpp \
    OsdabZip/zipglobal.cpp \
    OsdabZip/zip.cpp \
    OsdabZip/unzip.cpp \
    qwin/qwinwidget.cpp \
    qwin/qwinhost.cpp \
    qwin/qmfcapp.cpp \
    xmlwriter.cpp \
    xmlreader.cpp \
    treewidget.cpp \
    scriptgup.cpp \
    plugin.cpp \
    dockablewindow.cpp \
    archivereader.cpp \
    script_thread.cpp

HEADERS  += scripts_widget.h \
    OsdabZip/zlib.h \
    OsdabZip/zipglobal.h \
    OsdabZip/zipentry_p.h \
    OsdabZip/zip_p.h \
    OsdabZip/zip.h \
    OsdabZip/zconf.h \
    OsdabZip/unzip_p.h \
    OsdabZip/unzip.h \
    qwin/qwinwidget.h \
    qwin/qwinhost.h \
    qwin/qmfcapp.h \
    xmlwriter.h \
    xmlreader.h \
    treewidget.h \
    scriptgup.h \
    plugin.h \
    dockablewindow.h \
    constants.h \
    archivereader.h \
    script_thread.h

FORMS    += scriptswidget.ui

#
# 3ds max lib
#
LIBS += -luser32 \
    -lole32 \
    -loleaut32 \
    -lgdi32 \
    -lodbc32 \
    -lodbccp32 \
    -lcomctl32 \
    -lbmm \
    -lcore \
    -lmaxutil \
    -lmaxscrpt \
    -lgup \
    -lparamblk2
DEPENDPATH += windows.h
DEFINES -= UNICODE
win32:QMAKE_CXXFLAGS += -wd4100 -wd4819 -wd4099

win32: LIBS += -L$$PWD/../../../../../3DsMaxSDK/Max9SDK/maxsdk/lib
INCLUDEPATH += $$PWD/../../../../../3DsMaxSDK/Max9SDK/maxsdk/include
DEPENDPATH += $$PWD/../../../../../3DsMaxSDK/Max9SDK/maxsdk/include

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/OsdabZip/lib/ -lzlib
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/OsdabZip/lib/ -lzlibd

INCLUDEPATH += $$PWD/OsdabZip
DEPENDPATH += $$PWD/OsdabZip

OTHER_FILES += \
    cp.bat
