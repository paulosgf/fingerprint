QT += core gui widgets
TEMPLATE = app
TARGET = fingerprint

INCLUDEPATH += ../src
DEPENDPATH += ../src
VPATH += ../bin ../lib
QMAKE_CXXFLAGS += -Wall -O2
QMAKE_LFLAGS += -Wl,-rpath,/usr/local/lib

SOURCES += ../src/main.cpp\
        ../src/mainwindow.cpp

HEADERS += ../src/mainwindow.h

FORMS += ../src/mainwindow.ui

LIB_NAME = libfpengine.so
DESTDIR = ../bin/
BIN_DIR = /usr/local/bin/
LIB_DIR = /usr/local/lib/
DATA_DIR = /usr/local/fingerprint/
LIBS += -L../lib -lfpengine -lrt -ldl

target.files = $$TARGET
target.path = $$BIN_DIR
library.files = $$LIB_NAME
library.path = $$LIB_DIR
INSTALLS += target library
post_install.commands = mkdir -p $$DATA_DIR && chmod 770 $$DATA_DIR && \
chown root.users $$DATA_DIR && cp $$DESTDIR*.dat $$DATA_DIR 
#@echo "...Now add user to users group: sudo usermod -aG users $USER"
QMAKE_EXTRA_TARGETS += post_install
