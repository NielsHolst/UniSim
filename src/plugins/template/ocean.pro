include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ambrosia_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    even_odd.h \
    ocean_factory.h

SOURCES += \
    even_odd.cpp \
    ocean_factory.cpp
