include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = test_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    constant_world.h \
    cost.h \
    cost_error.h \
    crop.h \
    farm.h \
    life_cycle.h \
    life_stage.h \
    operation.h \
    publish.h \
    test_factory.h

SOURCES += \
    constant_world.cpp \
    cost.cpp \
    cost_error.cpp \
    crop.cpp \
    farm.cpp \
    life_cycle.cpp \
    life_stage.cpp \
    operation.cpp \
    publish.cpp \
    test_factory.cpp

