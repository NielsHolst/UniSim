include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = insecticide_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    application.h \
    compartment.h \
    insecticide_factory.h

SOURCES += \
    application.cpp \
    compartment.cpp \
    insecticide_factory.cpp
