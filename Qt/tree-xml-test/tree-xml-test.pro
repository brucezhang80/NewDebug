#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T10:45:08
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tree-xml-test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treeview_widget.cpp \
    docitem.cpp \
    docmodel.cpp

HEADERS  += mainwindow.h \
    treeview_widget.h \
    docitem.h \
    docmodel.h

FORMS    += mainwindow.ui
