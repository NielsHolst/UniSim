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
include ($${US_QWT}/qwt.pri)
include ($${US_GSL}/gsl.pri)

HEADERS += \
    $${US_BASE}/test/test_component.h \
    $${US_BASE}/test/test_identifier.h \
    $${US_BASE}/test/test_minimiser.h \
    $${US_BASE}/test/test_moments.h \
    $${US_BASE}/test/test_named_object.h \
    $${US_BASE}/test/test_name_list.h \
    $${US_BASE}/test/test_parameter.h \
    $${US_BASE}/test/test_parameter_vector.h \
    $${US_ENGINE}/test/test_simulation.h \
    $${US_ENGINE}/test/test_simulation_maker.h \
    $${US_ENGINE}/test/test_simulation_maker_from_file.h \
    $${US_PLUGINS}/unisim/anonymous_model.h \
    $${US_PLUGINS}/unisim/steps.h \
    test_numeric_limits.h

SOURCES += \
    $${US_BASE}/test/test_component.cpp \
    $${US_BASE}/test/test_identifier.cpp \
    $${US_BASE}/test/test_minimiser.cpp \
    $${US_BASE}/test/test_moments.cpp \
    $${US_BASE}/test/test_named_object.cpp \
    $${US_BASE}/test/test_name_list.cpp \
    $${US_BASE}/test/test_parameter.cpp \
    $${US_BASE}/test/test_parameter_vector.cpp \
    $${US_ENGINE}/test/test_simulation.cpp \
    $${US_ENGINE}/test/test_simulation_maker.cpp \
    $${US_ENGINE}/test/test_simulation_maker_from_file.cpp \
    $${US_PLUGINS}/unisim/anonymous_model.cpp \
    $${US_PLUGINS}/unisim/steps.cpp \
    test_numeric_limits.cpp \
    main.cpp
