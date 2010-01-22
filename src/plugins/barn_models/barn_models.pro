include($$(UNISIM_ROOT)/src/config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = barn_models$${SUFFIX_STR}
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

