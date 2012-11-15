TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += "/usr/local/boost_1_52_0"
LIBS += -I "/usr/local/boost_1_52_0"
QMAKE_CXXFLAGS = -std=c++0x

QMAKE_LIBS += -lgomp
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

