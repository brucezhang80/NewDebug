#-------------------------------------------------
#
# Project created by QtCreator 2013-10-24T19:45:59
#
#-------------------------------------------------
include(QScintilla/QScintilla.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MaxscriptEditor
TEMPLATE = app
DEFINES = QSCINTILLA_DLL SCI_LEXER
DEPENDPATH = $$PWD/QScintilla
SUBDIRS = QScintilla


SOURCES += main.cpp\
        MainWindow.cpp \
    CodeEditor.cpp \
    LexerCPP.cpp

HEADERS  += MainWindow.h \
    CodeEditor.h \
    LexerCPP.h \

FORMS    += MainWindow.ui

RESOURCES += \
    MaxscriptEditor.qrc

#win32:CONFIG(release, debug|release): LIBS += -L$$[QT_INSTALL_LIBS] -lqscintilla2
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$[QT_INSTALL_LIBS] -lqscintilla2d
