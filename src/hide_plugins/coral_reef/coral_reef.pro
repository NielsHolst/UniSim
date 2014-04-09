include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = coral_reef_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    logistic_growth.h \
    spawning.h \
    coral_reef_factory.h

SOURCES += \
    logistic_growth.cpp \
    spawning.cpp \
    coral_reef_factory.cpp
