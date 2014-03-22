#-------------------------------------------------
#
# Project created by QtCreator 2013-01-28T09:40:02
#
#-------------------------------------------------
QT += sql

win32:CONFIG(release, debug|release):TARGET = ModelManager
else:win32:CONFIG(debug, debug|release):TARGET = ModelManagerd
TEMPLATE = lib

DEFINES += MODELMANAGER_LIBRARY

DLLDESTDIR = $$PWD/../common/bin
DESTDIR = $$PWD/../common/lib

SOURCES += modelmanager.cpp \
    modeltreeview.cpp \
    treemodel.cpp \
    treeitem.cpp \
    addmodelwidget.cpp

HEADERS += modelmanager.h \
    modeltreeview.h \
    treemodel.h \
    treeitem.h \
    addmodelwidget.h

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
