QT += core gui widgets
TARGET = demo

INCLUDEPATH += ../
DEPENDPATH += ../
VPATH += ../../bin ../../lib
QMAKE_CXXFLAGS += -Wall -O2
QMAKE_LFLAGS += -Wl,-rpath,/usr/local/lib

SOURCES += main.cpp mainwindow.cpp

HEADERS += ../mainwindow.h

FORMS += ../mainwindow.ui

LIB_NAME = libfpengine.so
DESTDIR = ../../bin/
BIN_DIR = /usr/local/bin/
LIBS += -L../../lib -lfpengine -lrt -ldl

target.files = $$TARGET
target.path = $$BIN_DIR
library.files = $$LIB_NAME
INSTALLS = target
