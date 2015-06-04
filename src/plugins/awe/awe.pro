include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = awe_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)


HEADERS += \
    awe_factory.h \
    crop.h \
    instant_mortality.h \
    plant_growth_stage.h \
    publish.h \
    rotation.h\
    seed_bank.h \
    weather.h \
    weed.h

SOURCES += \
    awe_factory.cpp \
    crop.cpp \
    instant_mortality.cpp \
    plant_growth_stage.cpp \
    publish.cpp \
    rotation.cpp\
    seed_bank.cpp \
    weather.cpp \
    weed.cpp
