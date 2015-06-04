include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = strawberry_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    infection.h \
    leaf_production.h \
    publish.h \
    strawberry_factory.h

SOURCES += \
    infection.cpp \
    leaf_production.cpp \
    publish.cpp \
    strawberry_factory.cpp
