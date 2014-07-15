TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    AbstractProductA.cpp \
    AbstractProductB.cpp \
    ConcreteFactory1.cpp \
    ConcreteFactory2.cpp \
    ProductA1.cpp \
    ProductA2.cpp \
    ProductB1.cpp \
    ProductB2.cpp \
    AbstractFactory.cpp

HEADERS += \
    AbstractProductA.h \
    AbstractProductB.h \
    ConcreteFactory1.h \
    ConcreteFactory2.h \
    ProductA1.h \
    ProductA2.h \
    ProductB1.h \
    ProductB2.h \
    AbstractFactory.h
