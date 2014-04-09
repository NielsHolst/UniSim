include(../../config.pri)

TEMPLATE = app
# CONFIG += console - Tested how a console app would work
DESTDIR = $${US_APPLICATIONS}
TARGET = UniSim-$${UNISIM_VERSION}$${DEBUG_SUFFIX}

# Load application icon
RC_FILE = ../../resources/unisim.rc
 
# Load other resources
RESOURCES = ../../resources/unisim.qrc

# Libraries used
include ($${US_QWT}/qwt.pri)
include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include ($${US_TCLAP}/tclap.pri)

# Source code
HEADERS += \
    command_line.h \
    file_location_combo.h \
    file_location_dialog.h \
    file_locations_forgiving.h \
    file_locations_sub_window.h \
    file_locations_widget.h \
    graph_generator.h \
    live_simulation.h \
    main_window.h \
    stored_widget.h \
    sub_window.h \
    tree_view_widget.h \
    image_widget.h

SOURCES += \
    command_line.cpp \
    file_location_combo.cpp \
    file_location_dialog.cpp \
    file_locations_forgiving.cpp \
    file_locations_sub_window.cpp \
    file_locations_widget.cpp \
    graph_generator.cpp \
    live_simulation.cpp \
    main_window.cpp \
    stored_widget.cpp \
    sub_window.cpp \
    tree_view_widget.cpp \
    unisim.cpp \
    image_widget.cpp
