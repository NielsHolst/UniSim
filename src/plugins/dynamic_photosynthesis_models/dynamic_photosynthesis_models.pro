include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = dynamic_photosynthesis_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	dynamic_photosynthesis_model_maker.h \
	induction_state.h \
	weather.h

SOURCES += \
	dynamic_photosynthesis_model_maker.cpp \
	induction_state.cpp \
	weather.cpp

