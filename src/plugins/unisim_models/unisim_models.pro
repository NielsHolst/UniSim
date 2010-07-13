include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = unisim_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	stage.h \
	anonymous_model.h \
	calendar.h \
    day_degrees.h \
    days.h \
    lactin_time.h \
    photo_thermal_time.h \
    unisim_model_maker.h

SOURCES += \
    stage.cpp \
	anonymous_model.cpp \
	calendar.cpp \
    day_degrees.cpp \
    days.cpp \
    lactin_time.cpp \
    photo_thermal_time.cpp \
    unisim_model_maker.cpp
