# Make test application
include($$(UNISIM_ROOT)/src/config.pri)

TEMPLATE = app
DESTDIR = $${US_APPLICATIONS}
TARGET = test_$${SUFFIX_STR}
CONFIG += console
QT += testlib

# Load application icon
RC_FILE = ../unisim.rc

# Input
include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    $${US_BASE}/test/test_identifier.h \
    $${US_BASE}/test/test_parameters.h \
    $${US_BASE}/test/test_utilities.h \
    $${US_ENGINE}/test/test_model_maker.h \
    $${US_ENGINE}/test/test_simulation.h \
    $${US_ENGINE}/test/test_simulation_maker.h \
    $${US_ENGINE}/test/test_simulation_trickle.h \
    $${US_ENGINE}/test/trickle_box.h \
    $${US_ENGINE}/test/trickle_sequence.h \
    $${US_PLUGINS}/standard_controllers/simple_controller.h \
    $${US_PLUGINS}/standard_models/anonymous_model.h \
    $${US_PLUGINS}/standard_models/test/test_life_stage.h \
    $${US_PLUGINS}/standard_models/test/test_life_cycle.h \
	../graph_generator.h \
	../live_simulation.h \
	../log_base.h \
    frequency_distribution.h \
    test_frequency_distribution.h \
    test_live_simulation.h

SOURCES += \
    $${US_BASE}/test/test_identifier.cpp \
    $${US_BASE}/test/test_parameters.cpp \
    $${US_BASE}/test/test_utilities.cpp \
    $${US_ENGINE}/test/test_model_maker.cpp \
    $${US_ENGINE}/test/test_simulation.cpp \
    $${US_ENGINE}/test/test_simulation_maker.cpp \
    $${US_ENGINE}/test/test_simulation_trickle.cpp \
    $${US_ENGINE}/test/trickle_box.cpp \
    $${US_ENGINE}/test/trickle_sequence.cpp \
    $${US_PLUGINS}/standard_controllers/simple_controller.cpp \
    $${US_PLUGINS}/standard_models/anonymous_model.cpp \
    $${US_PLUGINS}/standard_models/test/test_life_stage.cpp \
    $${US_PLUGINS}/standard_models/test/test_life_cycle.cpp \
	../graph_generator.cpp \
	../live_simulation.cpp \
	../log_base.cpp \
    frequency_distribution.cpp \
    test_frequency_distribution.cpp \
    test_live_simulation.cpp \
	main.cpp 
