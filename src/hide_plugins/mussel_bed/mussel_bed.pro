include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = mussel_bed_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    mussel.h \
    mussel_bed_factory.h \
    mussel_growth_rate.h \
    salinity_scale.h \
    starfish.h \
    starfish_feeding_rate.h \
    starfish_growth_rate.h \
    temperature_stscale.h \
    thinning.h \
    salinity_stscale.h \
    temperature_scale.h

SOURCES += \
    mussel.cpp \
    mussel_bed_factory.cpp \
    mussel_growth_rate.cpp \
    temperature_scale.cpp \
    salinity_scale.cpp \
    starfish.cpp \
    starfish_feeding_rate.cpp \
    starfish_growth_rate.cpp \
    thinning.cpp \
    salinity_stscale.cpp \
    temperature_stscale.cpp

