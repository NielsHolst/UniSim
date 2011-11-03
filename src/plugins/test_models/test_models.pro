include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = test_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    test_model_maker.h \
    constant_world.h \
    life_cycle.h \
    life_stage.h \
    with_parameter_file.h

SOURCES += \
    test_model_maker.cpp \
    constant_world.cpp \
    life_cycle.cpp \
    life_stage.cpp \
    with_parameter_file.cpp

