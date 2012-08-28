include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = beehive_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    beehive_factory.h \
    fecundity.h \
    growth.h \
    life_stage.h \
    resource.h \
    small_hive_beetle.h

SOURCES += \
    beehive_factory.cpp \
    fecundity.cpp \
    growth.cpp \
    life_stage.cpp \
    resource.cpp \
    small_hive_beetle.cpp

