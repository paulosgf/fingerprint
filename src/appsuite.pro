QT += core gui widgets
TEMPLATE = subdirs
SUBDIRS = auth demo fingerprint
auth.depends = fingerprint
demo.depends = fingerprint

ICON = ../fingerprint.png
DESK = ../fingerprint.desktop
APP = /usr/share/applications/
DATA_DIR = /usr/local/fingerprint
postarget.commands = mkdir -p $$DATA_DIR && chmod 0775 $$DATA_DIR && chown root.users $$DATA_DIR && cp $$ICON $$DATA_DIR && cp $$DESK $$APP
QMAKE_EXTRA_TARGETS += postarget
postarget.depends = all