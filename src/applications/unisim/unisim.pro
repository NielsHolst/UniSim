include(../../config.pri)

TEMPLATE = app
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

# Source code
HEADERS += \
    file_location_combo.h \
    file_location_dialog.h \
    file_locations_forgiving.h \
    file_locations_sub_window.h \
    file_locations_widget.h \
    graph_generator.h \
    live_simulation.h \
    log_base.h \
    log_sub_window.h \
    main_window.h \
    stored_widget.h \
    sub_window.h \
    xml_editor.h \
    image_widget.h

SOURCES += \
    file_location_combo.cpp \
    file_location_dialog.cpp \
    file_locations_forgiving.cpp \
    file_locations_sub_window.cpp \
    file_locations_widget.cpp \
    graph_generator.cpp \
    live_simulation.cpp \
    log_base.cpp \
    log_sub_window.cpp \
    main_window.cpp \
    stored_widget.cpp \
    sub_window.cpp \
    unisim.cpp \
    xml_editor.cpp \
    image_widget.cpp
