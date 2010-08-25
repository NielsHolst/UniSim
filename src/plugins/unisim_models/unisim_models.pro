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
    random_base.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    unisim_model_maker.h

SOURCES += \
    stage.cpp \
	anonymous_model.cpp \
	calendar.cpp \
    day_degrees.cpp \
    days.cpp \
    lactin_time.cpp \
    photo_thermal_time.cpp \
    random_base.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    unisim_model_maker.cpp
