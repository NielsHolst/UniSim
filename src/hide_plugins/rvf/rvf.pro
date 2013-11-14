include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = rvf_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ../unisim/day_degrees.h \
    ../unisim/physiological_time.h \
    egg_time.h \
    aedes_fecundity.h \
    culex_fecundity.h \
    rvf_factory.h \
    water_level.h \
    inactiveegg.h \
    adult.h \
    mosquito_demand.h

SOURCES += \
    ../unisim/day_degrees.cpp \
    ../unisim/physiological_time.cpp \
    egg_time.cpp \
    aedes_fecundity.cpp \
    culex_fecundity.cpp \
    rvf_factory.cpp \
    water_level.cpp \
    inactiveegg.cpp \
    adult.cpp \
    mosquito_demand.cpp
