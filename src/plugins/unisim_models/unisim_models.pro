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
    exponential.h \
    fixed.h \
    hydro_thermal_time.h \
    lactin_time.h \
    photo_thermal_time.h \
    physiological_time.h \
    random_base.h \
    random_generator.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    records.h \
    run_iterator_fixed.h \
    scheduled.h \
    trigger_by_date.h \
    unisim_model_maker.h

SOURCES += \
    stage.cpp \
	anonymous_model.cpp \
	calendar.cpp \
    day_degrees.cpp \
    days.cpp \
    exponential.cpp \
    fixed.cpp \
    hydro_thermal_time.cpp \
    lactin_time.cpp \
    photo_thermal_time.cpp \
    physiological_time.cpp \
    random_base.cpp \
    random_generator.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    records.cpp \
    run_iterator_fixed.cpp \
    scheduled.cpp \
    trigger_by_date.cpp \
    unisim_model_maker.cpp
