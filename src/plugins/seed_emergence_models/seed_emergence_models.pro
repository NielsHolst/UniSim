include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = seed_emergence_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	seed_emergence_model_maker.h \
    cumulative_emergence.h \
	weather.h

SOURCES += \
	seed_emergence_model_maker.cpp \
    cumulative_emergence.cpp \
	weather.cpp

