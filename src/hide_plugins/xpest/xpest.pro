include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = xpest_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    aphid_assimilate.h \
    aphid_rue.h \
    crop_component.h \
    damage.h \
    day_degree_scale.h \
    growth.h \
    input_table.h \
    reduction.h \
    remaining_partition.h \
    remobilization.h \
    rust_damage.h \
    stage.h \
    weeds.h \
    xpest_factory.h

SOURCES += \
    aphid_assimilate.cpp \
    aphid_rue.cpp \
    crop_component.cpp \
    damage.cpp \
    day_degree_scale.cpp \
    growth.cpp \
    input_table.cpp \
    reduction.cpp \
    remaining_partition.cpp \
    remobilization.cpp \
    rust_damage.cpp \
    stage.cpp \
    weeds.cpp \
    xpest_factory.cpp
