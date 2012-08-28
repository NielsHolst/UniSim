include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ambrosia_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ambrosia_factory.h \
    plant.h\
    time.h

SOURCES += \
    ambrosia_factory.cpp \
    plant.cpp \
    time.cpp