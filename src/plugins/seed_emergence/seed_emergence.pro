include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = seed_emergence_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	seed_emergence_factory.h \
    cumulative_emergence.h \
	weather.h

SOURCES += \
	seed_emergence_factory.cpp \
    cumulative_emergence.cpp \
	weather.cpp

