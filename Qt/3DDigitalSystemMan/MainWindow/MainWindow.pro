#-------------------------------------------------
#
# Project created by QtCreator 2012-12-18T10:05:11
#
#-------------------------------------------------

QT       += core gui sql

TARGET = MainWindow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    toolbartabwidget.cpp \
    modeltabpage.cpp \
    scripttabpage.cpp \
    completelineedit.cpp \
    addmaxmodeldlg.cpp \
    slider.cpp \
    settingtabpage.cpp \
    thread.cpp \
    datatransfetabpage.cpp \
    areowidget.cpp

HEADERS  += mainwindow.h \
    toolbartabwidget.h \
    modeltabpage.h \
    scripttabpage.h \
    completelineedit.h \
    addmaxmodeldlg.h \
    slider.h \
    settingtabpage.h \
    thread.h \
    datatransfetabpage.h \
    areowidget.h

FORMS    += mainwindow.ui \
    addmaxmodeldlg.ui

RESOURCES += \
    MainWindow.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../common/lib/ -lDataTransfe
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../common/lib/ -lDataTransfed
else:symbian: LIBS += -lDataTransfe
else:unix: LIBS += -L$$PWD/../common/lib/ -lDataTransfe

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../common/lib/ -lModelManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../common/lib/ -lModelManagerd

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../common/lib/ -lScriptManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../common/lib/ -lScriptManagerd

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include
