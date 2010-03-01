include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = beehive_models$${SUFFIX_STR}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	beehive_model_maker.h \
	aethina.h

SOURCES += \
	beehive_model_maker.cpp \
	aethina.cpp

