include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = dynamic_photosynthesis_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    dynamic_photosynthesis_factory.h \
    leaf.h \
    publish.h

SOURCES += \
    dynamic_photosynthesis_factory.cpp \
    leaf.cpp \
    publish.cpp

