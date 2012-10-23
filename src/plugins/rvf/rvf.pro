include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = rvf_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    mosquito_fecundity.h \
    rvf_factory.h \
    water_level.h

SOURCES += \
    mosquito_fecundity.cpp \
    rvf_factory.cpp \
    water_level.cpp
