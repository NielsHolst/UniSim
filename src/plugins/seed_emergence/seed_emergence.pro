include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = seed_emergence_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    cumulative_emergence.h \
    publish.h \
    seed_emergence_factory.h \
    weather.h

SOURCES += \
    cumulative_emergence.cpp \
    publish.cpp \
    seed_emergence_factory.cpp \
    weather.cpp

