include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ambrosia_models$${SUFFIX_STR}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	ambrosia_model_maker.h \
    plant.h\
    time.h \
    weather.h

SOURCES += \
	ambrosia_model_maker.cpp \
    plant.cpp \
    time.cpp \
    weather.cpp

