include(../../../config.pri)

TEMPLATE = app
DESTDIR = $${US_APPLICATIONS}
TARGET = test-unisim-$${UNISIM_VERSION}$${DEBUG_SUFFIX}
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
    $${US_BASE}/test/test_parameter.h \
    $${US_BASE}/test/test_utilities.h \
    $${US_ENGINE}/test/test_model_maker.h \
    $${US_ENGINE}/test/test_simulation.h \
    $${US_ENGINE}/test/test_simulation_maker.h \
    $${US_ENGINE}/test/test_simulation_trickle.h \
    $${US_ENGINE}/test/trickle_box.h \
    $${US_ENGINE}/test/trickle_sequence.h \
    $${US_PLUGINS}/unisim_integrators/time_step_limited.h \
    $${US_PLUGINS}/unisim_models/anonymous_model.h \
    $${US_PLUGINS}/unisim_models/stage.h \
    $${US_PLUGINS}/unisim_models/test/test_calendar.h \
    $${US_PLUGINS}/unisim_models/test/test_life_stage.h \
    $${US_PLUGINS}/unisim_models/test/test_life_cycle.h \
    $${US_PLUGINS}/unisim_models/test/test_stage.h \
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
    $${US_BASE}/test/test_parameter.cpp \
    $${US_BASE}/test/test_utilities.cpp \
    $${US_ENGINE}/test/test_model_maker.cpp \
    $${US_ENGINE}/test/test_simulation.cpp \
    $${US_ENGINE}/test/test_simulation_maker.cpp \
    $${US_ENGINE}/test/test_simulation_trickle.cpp \
    $${US_ENGINE}/test/trickle_box.cpp \
    $${US_ENGINE}/test/trickle_sequence.cpp \
    $${US_PLUGINS}/unisim_integrators/time_step_limited.cpp \
    $${US_PLUGINS}/unisim_models/anonymous_model.cpp \
    $${US_PLUGINS}/unisim_models/stage.cpp \
    $${US_PLUGINS}/unisim_models/test/test_calendar.cpp \
    $${US_PLUGINS}/unisim_models/test/test_life_stage.cpp \
    $${US_PLUGINS}/unisim_models/test/test_life_cycle.cpp \
    $${US_PLUGINS}/unisim_models/test/test_stage.cpp \
    ../graph_generator.cpp \
	../live_simulation.cpp \
	../log_base.cpp \
    frequency_distribution.cpp \
    test_frequency_distribution.cpp \
    test_live_simulation.cpp \
    #test_plugins.cpp \
	main.cpp 
