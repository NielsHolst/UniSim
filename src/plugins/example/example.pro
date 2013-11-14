include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = example_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    logistic_growth.h \
    even_odd.h \
    example_factory.h

SOURCES += \
    logistic_growth.cpp \
    even_odd.cpp \
    example_factory.cpp
