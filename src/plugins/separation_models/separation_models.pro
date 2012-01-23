include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = separation_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	separation_model_maker.h \
	liquid.h \
	separator.h \
	solid.h

SOURCES += \
	separation_model_maker.cpp \
	liquid.cpp \
	separator.cpp \
	solid.cpp

