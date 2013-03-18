include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = vg_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    environment.h \
    sky.h \
    vg_factory.h

SOURCES += \
    environment.cpp \
    sky.cpp \
    vg_factory.cpp
