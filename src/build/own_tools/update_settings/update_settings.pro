include(../../../config.pri)

DESTDIR = ..
TARGET = update_settings
TEMPLATE = app
QT -= gui
CONFIG -= app_bundle
CONFIG += console
CONFIG += release

# Load application icon
RC_FILE = ../../../icon/unisim.rc

HEADERS += \
    $${US_BASE}/file_location_info.h \
    $${US_BASE}/version.h \
    ../common/common.h

SOURCES += \
    $${US_BASE}/file_location_info.cpp \
    $${US_BASE}/version.cpp \
    ../common/common.cpp \
    main.cpp
