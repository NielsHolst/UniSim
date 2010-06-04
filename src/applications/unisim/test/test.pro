include(../../../config.pri)

TEMPLATE = app
DESTDIR = $${US_APPLICATIONS}
TARGET = test_unisim_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += console
QT += testlib

# Load application icon
RC_FILE = ../../../icon/unisim.rc

# Input
include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    $${US_BASE}/test/test_component.h \
    $${US_BASE}/test/test_identifier.h \
    $${US_BASE}/test/test_parameters.h \
    $${US_BASE}/test/test_utilities.h \
    $${US_ENGINE}/test/test_model_maker.h \
    $${US_ENGINE}/test/test_simulation.h \
    $${US_ENGINE}/test/test_simulation_maker.h \
    $${US_ENGINE}/test/test_simulation_trickle.h \
    $${US_ENGINE}/test/trickle_box.h \
    $${US_ENGINE}/test/trickle_sequence.h \
    $${US_PLUGINS}/intercom_models/area.h \
    $${US_PLUGINS}/intercom_models/canopy_layer.h \
    $${US_PLUGINS}/intercom_models/community.h \
    $${US_PLUGINS}/intercom_models/light_components.h \
    $${US_PLUGINS}/intercom_models/organ.h \
    $${US_PLUGINS}/intercom_models/photosynthesis.h \
    $${US_PLUGINS}/intercom_models/photosynthetic_rate.h \
    $${US_PLUGINS}/intercom_models/plant.h \
    $${US_PLUGINS}/intercom_models/time_slice.h \
    $${US_PLUGINS}/intercom_models/weather.h \
    $${US_PLUGINS}/intercom_models/test/test_area.h \
    $${US_PLUGINS}/intercom_models/test/test_canopy_layer.h \
    $${US_PLUGINS}/intercom_models/test/test_photosynthesis.h \
    $${US_PLUGINS}/intercom_models/test/test_photosynthetic_rate.h \
    $${US_PLUGINS}/standard_integrators/time_step_limited.h \
    $${US_PLUGINS}/standard_models/anonymous_model.h \
    $${US_PLUGINS}/standard_models/calendar.h \
    $${US_PLUGINS}/standard_models/stage.h \
    $${US_PLUGINS}/standard_models/test/test_calendar.h \
    $${US_PLUGINS}/standard_models/test/test_life_stage.h \
    $${US_PLUGINS}/standard_models/test/test_life_cycle.h \
    $${US_PLUGINS}/standard_models/test/test_stage.h \
    ../graph_generator.h \
	../live_simulation.h \
	../log_base.h \
    frequency_distribution.h \
    test_frequency_distribution.h \
    test_live_simulation.h
    #test_plugins.h

SOURCES += \
    $${US_BASE}/test/test_component.cpp \
    $${US_BASE}/test/test_identifier.cpp \
    $${US_BASE}/test/test_parameters.cpp \
    $${US_BASE}/test/test_utilities.cpp \
    $${US_ENGINE}/test/test_model_maker.cpp \
    $${US_ENGINE}/test/test_simulation.cpp \
    $${US_ENGINE}/test/test_simulation_maker.cpp \
    $${US_ENGINE}/test/test_simulation_trickle.cpp \
    $${US_ENGINE}/test/trickle_box.cpp \
    $${US_ENGINE}/test/trickle_sequence.cpp \
    $${US_PLUGINS}/intercom_models/area.cpp \
    $${US_PLUGINS}/intercom_models/canopy_layer.cpp \
    $${US_PLUGINS}/intercom_models/community.cpp \
    $${US_PLUGINS}/intercom_models/light_components.cpp \
    $${US_PLUGINS}/intercom_models/organ.cpp \
    $${US_PLUGINS}/intercom_models/photosynthesis.cpp \
    $${US_PLUGINS}/intercom_models/photosynthetic_rate.cpp \
    $${US_PLUGINS}/intercom_models/plant.cpp \
    $${US_PLUGINS}/intercom_models/time_slice.cpp \
    $${US_PLUGINS}/intercom_models/weather.cpp \
    $${US_PLUGINS}/intercom_models/test/test_area.cpp \
    $${US_PLUGINS}/intercom_models/test/test_canopy_layer.cpp \
    $${US_PLUGINS}/intercom_models/test/test_photosynthesis.cpp \
    $${US_PLUGINS}/intercom_models/test/test_photosynthetic_rate.cpp \
    $${US_PLUGINS}/standard_integrators/time_step_limited.cpp \
    $${US_PLUGINS}/standard_models/anonymous_model.cpp \
    $${US_PLUGINS}/standard_models/calendar.cpp \
    $${US_PLUGINS}/standard_models/stage.cpp \
    $${US_PLUGINS}/standard_models/test/test_calendar.cpp \
    $${US_PLUGINS}/standard_models/test/test_life_stage.cpp \
    $${US_PLUGINS}/standard_models/test/test_life_cycle.cpp \
    $${US_PLUGINS}/standard_models/test/test_stage.cpp \
    ../graph_generator.cpp \
	../live_simulation.cpp \
	../log_base.cpp \
    frequency_distribution.cpp \
    test_frequency_distribution.cpp \
    test_live_simulation.cpp \
    #test_plugins.cpp \
	main.cpp 
