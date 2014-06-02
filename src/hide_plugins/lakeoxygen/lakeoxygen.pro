include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = lakeoxygen_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	lakeoxygen_factory.h \
	lake_environment.h \
    net_production.h \
    oxygen_concentration.h

SOURCES += \
	lakeoxygen_factory.cpp \
	lake_environment.cpp \
    net_production.cpp \
    oxygen_concentration.cpp

