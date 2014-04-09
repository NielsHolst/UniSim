include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = beehive_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ../unisim/distributed_delay_base.h \
    ../unisim/stage_base.h \
    beehive_factory.h \
    fecundity.h \
    growth.h \
    life_stage.h \
    queen_fecundity.h \
    queen_fecundity2.h \
    resource.h \
    small_hive_beetle.h

SOURCES += \
    ../unisim/distributed_delay_base.cpp \
    ../unisim/stage_base.cpp \
    beehive_factory.cpp \
    fecundity.cpp \
    growth.cpp \
    life_stage.cpp \
    queen_fecundity.cpp \
    queen_fecundity2.cpp \
    resource.cpp \
    small_hive_beetle.cpp

