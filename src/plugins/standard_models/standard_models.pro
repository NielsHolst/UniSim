include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = standard_models$${SUFFIX_STR}

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
    standard_model_maker.h

SOURCES += \
    stage.cpp \
	anonymous_model.cpp \
	calendar.cpp \
    day_degrees.cpp \
    days.cpp \
    lactin_time.cpp \
    photo_thermal_time.cpp \
    standard_model_maker.cpp
