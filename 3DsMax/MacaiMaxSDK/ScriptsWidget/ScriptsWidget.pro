#-------------------------------------------------
#
# Project created by QtCreator 2013-07-08T18:13:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScriptsWidget
TEMPLATE = app


SOURCES += main.cpp\
        scripts_widget.cpp \
    table_widget.cpp \
    xmlwriter.cpp \
    xmlreader.cpp \
    archivereader.cpp \
    OsdabZip/zipglobal.cpp \
    OsdabZip/zip.cpp \
    OsdabZip/unzip.cpp

HEADERS  += scripts_widget.h \
    table_widget.h \
    xmlwriter.h \
    xmlreader.h \
    constants.h \
    archivereader.h \
    OsdabZip/zlib.h \
    OsdabZip/zipglobal.h \
    OsdabZip/zipentry_p.h \
    OsdabZip/zip_p.h \
    OsdabZip/zip.h \
    OsdabZip/zconf.h \
    OsdabZip/unzip_p.h \
    OsdabZip/unzip.h

FORMS    += \
    scriptswidget.ui

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/OsdabZip/lib/ -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/OsdabZip/lib/ -lzlibd

INCLUDEPATH += $$PWD/OsdabZip
DEPENDPATH += $$PWD/OsdabZip
