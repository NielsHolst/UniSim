include(../../config.pri)

QT       += core
QT       -= gui

TARGET = UniSim2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = $${US_APPLICATIONS}
TARGET = UniSim2-$${UNISIM_VERSION}$${DEBUG_SUFFIX}

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)


SOURCES += main.cpp
