include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ambrosia_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ambrosia_time.h \
    ambrosia_factory.h \
    plant.h\

SOURCES += \
    ambrosia_time.cpp \
    ambrosia_factory.cpp \
    plant.cpp

