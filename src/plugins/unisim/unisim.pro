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
    anonymous.h \
    asymptotic_decreasing.h \
    asymptotic_increasing.h \
    calendar.h \
    day_degrees.h \
    days.h \
    distributed_delay.h \
    distributed_delay_2d.h \
    distributed_delay_base.h \
    exponential.h \
    fixed.h \
    forage.h \
    functional_response_g_b.h \
    functional_response_multiway.h \
    hydro_thermal_time.h \
    infection.h \
    inachis.h \
    insect_life_cycle.h \
    lactin_time.h \
    output_crosstab.h \
    output_map.cpp \
    output_plot.h \
    output_table.h \
    output_table_base.h \
    patch_square.h \
    photo_thermal_time.h \
    physiological_time.h \
    plot.h \
    predation.h \
    random_base.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    range.h \
    records.h \
    run_iterator_fixed.h \
    scenarios.h \
    scheduled.h \
    sensitivity_analysis.h \
    stage.h \
    stage_base.h \
    stage_demand.h \
    stage_and_phase.h \
    steps.h \
    sum.h \
    time_limited.h \
    trigger_by_date.h \
    unisim_factory.h \
    weather.h

SOURCES += \
    anonymous.cpp \
    asymptotic_decreasing.cpp \
    asymptotic_increasing.cpp \
    calendar.cpp \
    day_degrees.cpp \
    days.cpp \
    distributed_delay.cpp \
    distributed_delay_2d.cpp \
    distributed_delay_base.cpp \
    exponential.cpp \
    fixed.cpp \
    forage.cpp \
    functional_response_g_b.cpp \
    functional_response_multiway.cpp \
    hydro_thermal_time.cpp \
    infection.cpp \
    inachis.cpp \
    insect_life_cycle.cpp \
    lactin_time.cpp \
    output_crosstab.cpp \
    output_map.cpp \
    output_plot.cpp \
    output_table.cpp \
    output_table_base.cpp \
    patch_square.cpp \
    photo_thermal_time.cpp \
    physiological_time.cpp \
    plot.cpp \
    predation.cpp \
    random_base.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    range.cpp \
    records.cpp \
    run_iterator_fixed.cpp \
    scenarios.cpp \
    scheduled.cpp \
    sensitivity_analysis.cpp \
    stage.cpp \
    stage_base.cpp \
    stage_demand.cpp \
    stage_and_phase.cpp \
    steps.cpp \
    sum.cpp \
    time_limited.cpp \
    trigger_by_date.cpp \
    unisim_factory.cpp \
    weather.cpp
