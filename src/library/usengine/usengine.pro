include(../../config.pri)

TEMPLATE = lib
CONFIG += dll

DESTDIR = $${US_ENGINE}/..
TARGET = $${US_ENGINE_LIB_NAME}

# Libraries used
include ($${US_QWT}/qwt.pri)
include ($${US_BASE}/base.pri)

# Source code
HEADERS += 	\
    $${US_ENGINE}/builder.h \
    $${US_ENGINE}/confirmation.h \
    $${US_ENGINE}/documentation_writer.h \
    $${US_ENGINE}/file_locations_strict.h \
    $${US_ENGINE}/instance_index.h \
    $${US_ENGINE}/instance_index_from_condensed_table.h \
    $${US_ENGINE}/instance_index_from_table.h \
    $${US_ENGINE}/instance_index_numbered.h \
    $${US_ENGINE}/instance_index_one.h \
    $${US_ENGINE}/mega_factory.h \
    $${US_ENGINE}/parameter_index.h \
    $${US_ENGINE}/parameter_index_from_condensed_table.h \
    $${US_ENGINE}/parameter_index_from_table.h \
    $${US_ENGINE}/plot_widget.h \
    $${US_ENGINE}/simulation.h \
    $${US_ENGINE}/simulation_maker.h \
    $${US_ENGINE}/test_dataset.h \
    $${US_ENGINE}/xml_elements_reader.h \
    $${US_ENGINE}/xml_node.h

SOURCES += 	\
    $${US_ENGINE}/builder.cpp \
    $${US_ENGINE}/confirmation.cpp \
    $${US_ENGINE}/documentation_writer.cpp \
    $${US_ENGINE}/file_locations_strict.cpp \
    $${US_ENGINE}/instance_index.cpp \
    $${US_ENGINE}/instance_index_from_condensed_table.cpp \
    $${US_ENGINE}/instance_index_from_table.cpp \
    $${US_ENGINE}/instance_index_numbered.cpp \
    $${US_ENGINE}/instance_index_one.cpp \
    $${US_ENGINE}/mega_factory.cpp \
    $${US_ENGINE}/parameter_index.cpp \
    $${US_ENGINE}/parameter_index_from_condensed_table.cpp \
    $${US_ENGINE}/parameter_index_from_table.cpp \
    $${US_ENGINE}/plot_widget.cpp \
    $${US_ENGINE}/simulation.cpp \
    $${US_ENGINE}/simulation_maker.cpp \
    $${US_ENGINE}/test_dataset.cpp \
    $${US_ENGINE}/xml_elements_reader.cpp \
    $${US_ENGINE}/xml_node.cpp
