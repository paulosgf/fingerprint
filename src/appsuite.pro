QT += core gui widgets
TEMPLATE = subdirs
SUBDIRS = auth demo fingerprint
auth.depends = fingerprint
demo.depends = fingerprint

ICON = ../fingerprint.png
DESK = ../fingerprint.desktop
RULE = ../20-fingerprint.rules
APP = /usr/share/applications/
DATA_DIR = /usr/local/fingerprint
RULE_DIR = /etc/udev/rules.d

LIB_NAME = ../lib/libfpengine.so
LIB_DIR = /usr/local/lib
library.files = $$LIB_NAME
library.path = $$LIB_DIR

INSTALLS += library
postarget.commands = mkdir -p $$DATA_DIR && chmod 0775 $$DATA_DIR && chown root.users $$DATA_DIR && cp $$ICON $$DATA_DIR && cp $$DESK $$APP && cp $$RULE $$RULE_DIR
QMAKE_EXTRA_TARGETS += postarget
postarget.depends = all
