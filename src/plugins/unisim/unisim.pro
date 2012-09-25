include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = unisim_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include ($${US_GSL}/gsl.pri)
include ($${US_QWT}/qwt.pri)

HEADERS += \
    anonymous_model.h \
    calendar.h \
    day_degrees.h \
    days.h \
    distributed_delay.h \
    exponential.h \
    fixed.h \
    functional_response_g_b.h \
    hydro_thermal_time.h \
    inachis.h \
    insect_life_cycle.h \
    lactin_time.h \
    output_crosstab.h \
    output_plot.h \
    output_table.h \
    output_table_base.h \
    photo_thermal_time.h \
    physiological_time.h \
    plot.h \
    random_base.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    range.h \
    records.h \
    run_iterator_fixed.h \
    scheduled.h \
    sensitivity_analysis.h \
    stage.h \
    stage_demand.h \
    steps.h \
    time_limited.h \
    trigger_by_date.h \
    unisim_factory.h \
    weather.h

SOURCES += \
    anonymous_model.cpp \
    calendar.cpp \
    day_degrees.cpp \
    days.cpp \
    distributed_delay.cpp \
    exponential.cpp \
    fixed.cpp \
    functional_response_g_b.cpp \
    hydro_thermal_time.cpp \
    inachis.cpp \
    insect_life_cycle.cpp \
    lactin_time.cpp \
    output_crosstab.cpp \
    output_plot.cpp \
    output_table.cpp \
    output_table_base.cpp \
    photo_thermal_time.cpp \
    physiological_time.cpp \
    plot.cpp \
    random_base.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    range.cpp \
    records.cpp \
    run_iterator_fixed.cpp \
    scheduled.cpp \
    sensitivity_analysis.cpp \
    stage.cpp \
    stage_demand.cpp \
    steps.cpp \
    time_limited.cpp \
    trigger_by_date.cpp \
    unisim_factory.cpp \
    weather.cpp
