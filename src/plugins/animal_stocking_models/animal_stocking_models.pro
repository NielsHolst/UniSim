include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = AnimalStocking_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	animal_stocking_model_maker.h \
	environment.h \
	grass_land.h \
	grass_productivity.h \
        grass_productivity_cutting_estimate.h \
        grass_productivity_remote_estimate.h \
        stocking_capacity.h

SOURCES += \
	animal_stocking_model_maker.cpp \
	environment.cpp \
	grass_land.cpp \
	grass_productivity.cpp \
        grass_productivity_cutting_estimate.cpp \
        grass_productivity_remote_estimate.cpp \
        stocking_capacity.cpp

