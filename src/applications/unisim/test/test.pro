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
    $${US_BASE}/test/test_data_grid.h \
    $${US_BASE}/test/test_identifier.h \
    $${US_BASE}/test/test_interpolate.h \
    $${US_BASE}/test/test_minimiser.h \
    $${US_BASE}/test/test_matrix.h \
    $${US_BASE}/test/test_moments.h \
    $${US_BASE}/test/test_named_object.h \
    $${US_BASE}/test/test_name_list.h \
    $${US_BASE}/test/test_parameter.h \
    $${US_BASE}/test/test_parameter_vector.h \
    $${US_BASE}/test/test_push_variable.h \
    $${US_BASE}/test/test_reference.h \
    $${US_BASE}/test/test_root_solver.h \
    $${US_BASE}/test/test_strata.h \
    $${US_BASE}/test/test_time.h \
    $${US_BASE}/test/test_utilities.h \
    $${US_ENGINE}/test/test_ast.h \
    $${US_ENGINE}/test/test_builder.h \
    $${US_ENGINE}/test/test_mega_factory.h \
    $${US_ENGINE}/test/test_simulation.h \
    $${US_ENGINE}/test/test_simulation_maker.h \
    $${US_ENGINE}/test/test_simulation_maker_from_file.h \
    $${US_ENGINE}/test/test_xml_node.h \
#    $${US_PLUGINS}/intercom/optimize_allocation.h \
#    $${US_PLUGINS}/intercom/test/test_plant.h \
    $${US_PLUGINS}/unisim/distributed_delay.h \
    $${US_PLUGINS}/unisim/distributed_delay_2d.h \
    $${US_PLUGINS}/unisim/distributed_delay_base.h \
    $${US_PLUGINS}/unisim/forage.h \
    $${US_PLUGINS}/unisim/stage_base.h \
    $${US_PLUGINS}/unisim/test/test_calendar.h \
    $${US_PLUGINS}/unisim/test/test_distributed_delay.h \
    $${US_PLUGINS}/unisim/test/test_distributed_delay_2d.h \
    $${US_PLUGINS}/unisim/test/test_functional_response_g_b.h \
    $${US_PLUGINS}/unisim/test/test_fixed.h \
    $${US_PLUGINS}/unisim/test/test_forage.h \
    $${US_PLUGINS}/unisim/test/test_infection.h \
    $${US_PLUGINS}/unisim/test/test_predation.h \
    $${US_PLUGINS}/unisim/test/test_random.h \
    $${US_PLUGINS}/unisim/test/test_range.h \
    $${US_PLUGINS}/unisim/test/test_records.h \
    $${US_PLUGINS}/unisim/test/test_scheduled.h \
    $${US_PLUGINS}/unisim/test/test_stage.h \
    $${US_PLUGINS}/unisim/test/test_sum.h \
#    $${US_PLUGINS}/unisim/output_plot.h \
#    $${US_PLUGINS}/unisim/plot.h \
    $${US_PLUGINS}/unisim/test/test_crosstab.h \
#    $${US_PLUGINS}/unisim/test/test_output_plot.h \
#    $${US_PLUGINS}/unisim/test/test_plot.h \
    $${US_PLUGINS}/unisim/test/test_table.h \
    $${US_PLUGINS}/vg/test/create.h \
    $${US_PLUGINS}/vg/test/test_adjustable_setpoint.h \
    $${US_PLUGINS}/vg/test/test_construction_geometry.h \
    $${US_PLUGINS}/vg/test/test_transmission.h \
    ../graph_generator.h \
    main_window_stub.h \
    test_numeric_limits.h

SOURCES += \
    $${US_BASE}/test/test_component.cpp \
    $${US_BASE}/test/test_data_grid.cpp \
    $${US_BASE}/test/test_identifier.cpp \
    $${US_BASE}/test/test_interpolate.cpp \
    $${US_BASE}/test/test_matrix.cpp \
    $${US_BASE}/test/test_minimiser.cpp \
    $${US_BASE}/test/test_moments.cpp \
    $${US_BASE}/test/test_named_object.cpp \
    $${US_BASE}/test/test_name_list.cpp \
    $${US_BASE}/test/test_parameter.cpp \
    $${US_BASE}/test/test_parameter_vector.cpp \
    $${US_BASE}/test/test_push_variable.cpp \
    $${US_BASE}/test/test_root_solver.cpp \
    $${US_BASE}/test/test_strata.cpp \
    $${US_BASE}/test/test_reference.cpp \
    $${US_BASE}/test/test_time.cpp \
    $${US_BASE}/test/test_utilities.cpp \
    $${US_ENGINE}/test/test_ast.cpp \
    $${US_ENGINE}/test/test_builder.cpp \
    $${US_ENGINE}/test/test_mega_factory.cpp \
    $${US_ENGINE}/test/test_simulation.cpp \
    $${US_ENGINE}/test/test_simulation_maker.cpp \
    $${US_ENGINE}/test/test_simulation_maker_from_file.cpp \
    $${US_ENGINE}/test/test_xml_node.cpp \
#    $${US_PLUGINS}/intercom/optimize_allocation.cpp \
#    $${US_PLUGINS}/intercom/test/test_plant.cpp \
    $${US_PLUGINS}/unisim/distributed_delay.cpp \
    $${US_PLUGINS}/unisim/distributed_delay_2d.cpp \
    $${US_PLUGINS}/unisim/distributed_delay_base.cpp \
    $${US_PLUGINS}/unisim/forage.cpp \
    $${US_PLUGINS}/unisim/stage_base.cpp \
    $${US_PLUGINS}/unisim/test/test_calendar.cpp \
    $${US_PLUGINS}/unisim/test/test_distributed_delay.cpp \
    $${US_PLUGINS}/unisim/test/test_distributed_delay_2d.cpp \
    $${US_PLUGINS}/unisim/test/test_functional_response_g_b.cpp \
    $${US_PLUGINS}/unisim/test/test_fixed.cpp \
    $${US_PLUGINS}/unisim/test/test_forage.cpp \
    $${US_PLUGINS}/unisim/test/test_infection.cpp \
    $${US_PLUGINS}/unisim/test/test_predation.cpp \
    $${US_PLUGINS}/unisim/test/test_random.cpp \
    $${US_PLUGINS}/unisim/test/test_range.cpp \
    $${US_PLUGINS}/unisim/test/test_records.cpp \
    $${US_PLUGINS}/unisim/test/test_scheduled.cpp \
    $${US_PLUGINS}/unisim/test/test_stage.cpp \
    $${US_PLUGINS}/unisim/test/test_sum.cpp \
#    $${US_PLUGINS}/unisim/output_plot.cpp \
#    $${US_PLUGINS}/unisim/plot.cpp \
    $${US_PLUGINS}/unisim/test/test_crosstab.cpp \
#    $${US_PLUGINS}/unisim/test/test_output_plot.cpp \
#    $${US_PLUGINS}/unisim/test/test_plot.cpp \
    $${US_PLUGINS}/unisim/test/test_table.cpp \
    $${US_PLUGINS}/vg/test/create.cpp \
    $${US_PLUGINS}/vg/test/test_adjustable_setpoint.cpp \
    $${US_PLUGINS}/vg/test/test_construction_geometry.cpp \
    $${US_PLUGINS}/vg/test/test_transmission.cpp \
    ../graph_generator.cpp \
    test_numeric_limits.cpp \
    main.cpp
