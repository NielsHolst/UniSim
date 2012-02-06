include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = cotton_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    cotton_model_maker.h \
    insect.h

SOURCES += \
    cotton_model_maker.cpp \
    insect.cpp
