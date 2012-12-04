include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = diffevol_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    differential_evolution.h \
    plot.h \
    diffevol_factory.h \
    year.h

SOURCES += \
    differential_evolution.cpp \
    plot.cpp \
    diffevol_factory.cpp \
    year.cpp
