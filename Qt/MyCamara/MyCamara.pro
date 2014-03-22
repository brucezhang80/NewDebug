#-------------------------------------------------
#
# Project created by QtCreator 2013-11-17T14:03:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyCamara
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

#opencv
INCLUDEPATH += $$PWD/../../3rdLibrary/opencv/build/include \
            $$PWD/../../3rdLibrary/opencv/build/include/opencv \
            $$PWD/../../3rdLibrary/opencv/build/include/opencv2

DEPENDPATH += $$PWD/../../3rdLibrary/opencv/build/x86/vc11 \

LIBS += -L$$PWD/../../3rdLibrary/opencv/build/x86/vc11/lib/

win32: LIBS += -lopencv_core247d \
            -lopencv_highgui247d \
            -lopencv_photo247d \
            -lopencv_imgproc247d \
            -lshell32

#win32: LIBS += -L$$PWD/../../3rdLibrary/opencv/build/x86/vc11/lib/ -lopencv_highgui247d

