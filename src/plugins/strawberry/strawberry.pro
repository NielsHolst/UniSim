include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ambrosia_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    even_odd.h \
    strawberry_factory.h

SOURCES += \
    even_odd.cpp \
    strawberry_factory.cpp
