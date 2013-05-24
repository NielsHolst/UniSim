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
    temperature_scale.h \
    salinity_scale.h \
    starfish.h \
    starfish_feeding_rate.h \
    starfish_growth_rate.h \
    temperature_stscale.h \
    starfish_salinity_mortality.h \
    searchrate.h \
    hydrodynamic_SRscale.h \
    salinity_SRscale.h \
    thinning.h

SOURCES += \
    mussel.cpp \
    mussel_bed_factory.cpp \
    mussel_growth_rate.cpp \
    temperature_scale.cpp \
    salinity_scale.cpp \
    starfish.cpp \
    starfish_feeding_rate.cpp \
    starfish_growth_rate.cpp \
    temperature_stscale.cpp \
    starfish_salinity_mortality.cpp \
    searchrate.cpp \
    hydrodynamic_SRscale.cpp \
    salinity_SRscale.cpp \
    thinning.cpp

