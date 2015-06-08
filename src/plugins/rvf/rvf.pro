include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = rvf_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    egg_time.h \
    interactions.h \
    mosquito_demand.h \
    mosquito_fecundity.h \
    mosquito_infection.h \
    publish.h \
    rvf_factory.h \
    sheep_fecundity.h \
    sheep_infection.h \
    water_level.h

SOURCES += \
    egg_time.cpp \
    interactions.cpp \
    mosquito_demand.cpp \
    mosquito_fecundity.cpp \
    mosquito_infection.cpp \
    publish.cpp \
    rvf_factory.cpp \
    sheep_fecundity.cpp \
    sheep_infection.cpp \
    water_level.cpp
