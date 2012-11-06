include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = network_indicators_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    farm.h \
    import_indicator.h \
    network_indicators_factory.h

SOURCES += \
    farm.cpp \
    import_indicator.cpp \
    network_indicators_factory.cpp
