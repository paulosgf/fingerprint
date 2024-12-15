QT += core gui widgets
TARGET = fingerprint

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
LIB_DIR = /usr/local/lib/
LIBS += -L../../lib -lfpengine -lrt -ldl

target.files = $$TARGET
target.path = $$BIN_DIR
library.files = $$LIB_NAME
library.path = $$LIB_DIR
INSTALLS += target library

DATA_DIR = /usr/local/fingerprint
postarget.commands = mkdir -p $$DATA_DIR && chmod 0770 $$DATA_DIR && chown root.users $$DATA_DIR
QMAKE_EXTRA_TARGETS += postarget
