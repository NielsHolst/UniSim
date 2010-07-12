include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = awe_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)


HEADERS += \
    crop.h \
    awe_model_maker.h \
    instant_mortality.h \
	plant_growth_stage.h \
    rotation.h\
    seed_bank.h \
	weather.h \
	weed.h

SOURCES += \
    crop.cpp \
    awe_model_maker.cpp \
    instant_mortality.cpp \
    plant_growth_stage.cpp \
    rotation.cpp\
    seed_bank.cpp \
    weather.cpp \
	weed.cpp
