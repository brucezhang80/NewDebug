#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T15:50:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeskCalendar
TEMPLATE = app


SOURCES += main.cpp \
    MainWidget.cpp \
    CalendarWidget.cpp

HEADERS  += \
    MainWidget.h \
    CalendarWidget.h

FORMS    += \
    MainWidget.ui

DEFINES -= UNICODE
LIBS += -luser32
