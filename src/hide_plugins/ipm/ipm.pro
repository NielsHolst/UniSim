include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ipm_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ipm_factory.h \
    even_odd.h

SOURCES += \
    ipm_factory.cpp \
    even_odd.cpp
