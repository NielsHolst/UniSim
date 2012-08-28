include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = awe_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)


HEADERS += \
    crop.h \
    awe_factory.h \
    instant_mortality.h \
	plant_growth_stage.h \
    rotation.h\
    seed_bank.h \
	weather.h \
	weed.h

SOURCES += \
    crop.cpp \
    awe_factory.cpp \
    instant_mortality.cpp \
    plant_growth_stage.cpp \
    rotation.cpp\
    seed_bank.cpp \
    weather.cpp \
	weed.cpp
