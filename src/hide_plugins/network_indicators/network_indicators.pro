include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = network_indicators_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    compartmental_throughflow.h \
    farm.h \
    import_indicator.h \
    indicator_base.h \
    network_indicators_factory.h

SOURCES += \
    compartmental_throughflow.cpp \
    farm.cpp \
    import_indicator.cpp \
    indicator_base.cpp \
    network_indicators_factory.cpp
