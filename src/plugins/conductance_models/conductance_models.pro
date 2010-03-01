include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = conductance_models$${SUFFIX_STR}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	conductance_model_maker.h \
	community.h \
	phase.h \
	plant.h \
	weather.h

SOURCES += \
	conductance_model_maker.cpp \
	community.cpp \
	plant.cpp \
	weather.cpp

