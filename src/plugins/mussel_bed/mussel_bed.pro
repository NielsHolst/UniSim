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
    temperature_scale.h

SOURCES += \
    mussel.cpp \
    mussel_bed_factory.cpp \
    mussel_growth_rate.cpp \
    temperature_scale.cpp

