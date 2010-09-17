include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = barn_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	barn_model_maker.h \
	predator.h \
	prey.h

SOURCES += \
	barn_model_maker.cpp \
	predator.cpp \
	prey.cpp

