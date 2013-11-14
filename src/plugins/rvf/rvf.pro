include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = rvf_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    sheep_infection.h \
    mosquito_infection.h \
    sheep_fecundity.h \
    mosquito_fecundity.h \
    rvf_factory.h \
    water_level.h

SOURCES += \
    sheep_infection.cpp \
    mosquito_infection.cpp \
    sheep_fecundity.cpp \
    mosquito_fecundity.cpp \
    rvf_factory.cpp \
    water_level.cpp
