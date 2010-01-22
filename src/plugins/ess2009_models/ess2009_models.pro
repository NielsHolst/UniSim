# Make ess2009_models plugin library
include($$(UNISIM_ROOT)/src/config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ess2009_models$${SUFFIX_STR}
CONFIG += plugin

include ($${US_BASE}/base.pri)


HEADERS += \
    crop.h \
    ess2009_model_maker.h \
    instant_mortality.h \
	plant_growth_stage.h \
    rotation.h\
    seed_bank.h \
    treatments.h \
	weather.h \
	weed.h

SOURCES += \
    crop.cpp \
    ess2009_model_maker.cpp \
    instant_mortality.cpp \
    plant_growth_stage.cpp \
    rotation.cpp\
    seed_bank.cpp \
    treatments.cpp \
    weather.cpp \
	weed.cpp
