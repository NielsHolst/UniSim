include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = conductance_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	conductance_factory.h \
	community.h \
	phase.h \
	plant.h \
	weather.h

SOURCES += \
	conductance_factory.cpp \
	community.cpp \
	plant.cpp \
	weather.cpp

