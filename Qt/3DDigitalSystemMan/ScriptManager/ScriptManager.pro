#-------------------------------------------------
#
# Project created by QtCreator 2013-01-28T10:15:19
#
#-------------------------------------------------

QT       += sql

win32:CONFIG(release, debug|release):TARGET = ScriptManager
else:win32:CONFIG(debug, debug|release):TARGET = ScriptManagerd

TEMPLATE = lib

DEFINES += SCRIPTMANAGER_LIBRARY

DLLDESTDIR = $$PWD/../common/bin
DESTDIR = $$PWD/../common/lib

SOURCES += scriptmanager.cpp \
    scripttreeview.cpp \
    treemodel.cpp \
    treeitem.cpp \
    addscriptwidget.cpp

HEADERS += scriptmanager.h\
    scripttreeview.h \
    treemodel.h \
    treeitem.h \
    addscriptwidget.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    copy.bat
