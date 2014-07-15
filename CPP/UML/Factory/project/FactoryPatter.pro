TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ConcreteFactory.cpp \
    ConcreteProduct1.cpp \
    ConcreteProduct2.cpp \
    Factory.cpp \
    Product.cpp

HEADERS += \
    ConcreteFactory.h \
    ConcreteProduct1.h \
    ConcreteProduct2.h \
    Factory.h \
    Product.h

