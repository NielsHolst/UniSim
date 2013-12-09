include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = rvf_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ../unisim/day_degrees.h \
    ../unisim/physiological_time.h \
    interactions.h \
    egg_time.h \
    sheep_infection.h \
    mosquito_infection.h \
    sheep_fecundity.h \
    mosquito_demand.h \
    mosquito_fecundity.h \
    rvf_factory.h \
    water_level.h

SOURCES += \
    ../unisim/day_degrees.cpp \
    ../unisim/physiological_time.cpp \
    interactions.cpp \
    egg_time.cpp \
    sheep_infection.cpp \
    mosquito_infection.cpp \
    sheep_fecundity.cpp \
    mosquito_demand.cpp \
    mosquito_fecundity.cpp \
    rvf_factory.cpp \
    water_level.cpp
