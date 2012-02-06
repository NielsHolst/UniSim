include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = lakeoxygen_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	lakeoxygen_model_maker.h \
	lake_environment.h \
    net_production.h \
    oxygen_concentration.h

SOURCES += \
	lakeoxygen_model_maker.cpp \
	lake_environment.cpp \
    net_production.cpp \
    oxygen_concentration.cpp

