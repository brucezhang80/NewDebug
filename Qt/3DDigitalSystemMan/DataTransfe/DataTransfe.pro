#-------------------------------------------------
#
# Project created by QtCreator 2013-01-23T09:49:40
#
#-------------------------------------------------

win32:CONFIG(release, debug|release):TARGET = DataTransfe
else:win32:CONFIG(debug, debug|release):TARGET = DataTransfed

TEMPLATE = lib

DEFINES += DATATRANSFE_LIBRARY

DLLDESTDIR = $$PWD/../common/bin
DESTDIR = $$PWD/../common/lib

SOURCES += datatransfe.cpp \
    datatransfeoptions.cpp \
    datatransfeplan.cpp \
    datatransfehistory.cpp \
    dataclient.cpp \
    filemanager.cpp \
    qtfilecopier.cpp

HEADERS += datatransfe.h \
    datatransfeoptions.h \
    datatransfeplan.h \
    datatransfehistory.h \
    dataclient.h \
    filemanager.h \
    qtfilecopier.h

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

FORMS += \
    datatransfeoptions.ui
