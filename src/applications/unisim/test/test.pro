include(../../../config.pri)

TEMPLATE = app
DESTDIR = $${US_APPLICATIONS}
TARGET = test-unisim-$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += console
QT += testlib

# Load application icon
RC_FILE = ../../../resources/unisim.rc
 
# Load other resources
RESOURCES = ../../../resources/unisim.qrc

# Input
include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include($${US_QWT}/qwt.pri)

HEADERS += \
    $${US_BASE}/test/test_component.h \
    $${US_BASE}/test/test_identifier.h \
    $${US_BASE}/test/test_name_list.h \
    $${US_BASE}/test/test_parameter.h \
    $${US_BASE}/test/test_parameter_file.h \
    $${US_BASE}/test/test_push_variable.h \
    $${US_BASE}/test/test_strata.h \
    $${US_BASE}/test/test_time.h \
    $${US_BASE}/test/test_utilities.h \
    $${US_ENGINE}/test/test_model_maker.h \
    $${US_ENGINE}/test/test_simulation.h \
    $${US_ENGINE}/test/test_simulation_maker.h \
    $${US_ENGINE}/test/test_simulation_trickle.h \
    $${US_ENGINE}/test/trickle_box.h \
    $${US_ENGINE}/test/trickle_sequence.h \
    $${US_ENGINE}/test/test_xml_node.h \
    $${US_PLUGINS}/intercom_models/optimize_allocation.h \
    $${US_PLUGINS}/intercom_models/test/test_plant.h \
    $${US_PLUGINS}/test_models/with_parameter_file.h \
    $${US_PLUGINS}/unisim_integrators/time_step_limited.h \
    $${US_PLUGINS}/unisim_models/anonymous_model.h \
    $${US_PLUGINS}/unisim_models/stage.h \
    $${US_PLUGINS}/unisim_models/test/test_calendar.h \
    $${US_PLUGINS}/unisim_models/test/test_fixed.h \
    $${US_PLUGINS}/unisim_models/test/test_life_stage.h \
    $${US_PLUGINS}/unisim_models/test/test_life_cycle.h \
    $${US_PLUGINS}/unisim_models/test/test_records.h \
    $${US_PLUGINS}/unisim_models/test/test_scheduled.h \
    $${US_PLUGINS}/unisim_models/test/test_stage.h \
    $${US_PLUGINS}/unisim_outputs/plot.h \
    $${US_PLUGINS}/unisim_outputs/test/test_plot.h \
    ../graph_generator.h \
    frequency_distribution.h \
    test_frequency_distribution.h \
    test_numeric_limits.h
    #test_live_simulation.h
    #test_plugins.h

SOURCES += \
    $${US_BASE}/test/test_component.cpp \
    $${US_BASE}/test/test_identifier.cpp \
    $${US_BASE}/test/test_name_list.cpp \
    $${US_BASE}/test/test_parameter.cpp \
    $${US_BASE}/test/test_parameter_file.cpp \
    $${US_BASE}/test/test_push_variable.cpp \
    $${US_BASE}/test/test_strata.cpp \
    $${US_BASE}/test/test_time.cpp \
    $${US_BASE}/test/test_utilities.cpp \
    $${US_ENGINE}/test/test_model_maker.cpp \
    $${US_ENGINE}/test/test_simulation.cpp \
    $${US_ENGINE}/test/test_simulation_maker.cpp \
    $${US_ENGINE}/test/test_simulation_trickle.cpp \
    $${US_ENGINE}/test/trickle_box.cpp \
    $${US_ENGINE}/test/trickle_sequence.cpp \
    $${US_ENGINE}/test/test_xml_node.cpp \
    $${US_PLUGINS}/intercom_models/optimize_allocation.cpp \
    $${US_PLUGINS}/intercom_models/test/test_plant.cpp \
    $${US_PLUGINS}/test_models/with_parameter_file.cpp \
    $${US_PLUGINS}/unisim_integrators/time_step_limited.cpp \
    $${US_PLUGINS}/unisim_models/anonymous_model.cpp \
    $${US_PLUGINS}/unisim_models/stage.cpp \
    $${US_PLUGINS}/unisim_models/test/test_calendar.cpp \
    $${US_PLUGINS}/unisim_models/test/test_fixed.cpp \
    $${US_PLUGINS}/unisim_models/test/test_life_stage.cpp \
    $${US_PLUGINS}/unisim_models/test/test_life_cycle.cpp \
    $${US_PLUGINS}/unisim_models/test/test_records.cpp \
    $${US_PLUGINS}/unisim_models/test/test_scheduled.cpp \
    $${US_PLUGINS}/unisim_models/test/test_stage.cpp \
    $${US_PLUGINS}/unisim_outputs/plot.cpp \
    $${US_PLUGINS}/unisim_outputs/test/test_plot.cpp \
    ../graph_generator.cpp \
    frequency_distribution.cpp \
    test_frequency_distribution.cpp \
    test_numeric_limits.cpp \
    #test_live_simulation.cpp \
    #test_plugins.cpp \
	main.cpp 
