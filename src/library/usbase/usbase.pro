include(../../config.pri)

TEMPLATE = lib
CONFIG += dll

DESTDIR = $${US_BASE}/..
TARGET = $${US_BASE_LIB_NAME}

# Libraries used
include ($${US_GSL}/gsl.pri)

# Source code
HEADERS += \
        $${US_BASE}/authors.h \
        $${US_BASE}/attributes.h \
        $${US_BASE}/clock.h \
        $${US_BASE}/colours.h \
        $${US_BASE}/component.h \
        $${US_BASE}/data_grid.h \
        $${US_BASE}/date_interval.h \
        $${US_BASE}/exception.h \
        $${US_BASE}/factory_plug_in.h \
        $${US_BASE}/file_location_info.h \
        $${US_BASE}/file_locations.h \
        $${US_BASE}/identifier.h \
        $${US_BASE}/integrator.h \
        $${US_BASE}/main_window_interface.h \
        $${US_BASE}/matrix.h \
        $${US_BASE}/minimiser.h \
        $${US_BASE}/model.h \
        $${US_BASE}/moments.h \
        $${US_BASE}/named_object.h \
        $${US_BASE}/name_list.h \
        $${US_BASE}/object_pool.h \
        $${US_BASE}/output.h \
        $${US_BASE}/parameter.h \
        $${US_BASE}/parameter_base.h \
        $${US_BASE}/parameter_vector.h \
        $${US_BASE}/product_base.h \
        $${US_BASE}/product.h \
        $${US_BASE}/variable.h \
        $${US_BASE}/variable_base.h \
        $${US_BASE}/random.h \
        $${US_BASE}/root_solver.h \
        $${US_BASE}/strata.h \
        $${US_BASE}/strata_base.h \
        $${US_BASE}/test_num.h \
        $${US_BASE}/ustime.h \
        $${US_BASE}/trace.h \
        $${US_BASE}/utilities.h \
        $${US_BASE}/variable.h \
        $${US_BASE}/variable_base.h \
        $${US_BASE}/variable_vector.h \
        $${US_BASE}/version.h
SOURCES += \
        $${US_BASE}/authors.cpp \
        $${US_BASE}/clock.cpp \
        $${US_BASE}/colours.cpp \
        $${US_BASE}/component.cpp \
        $${US_BASE}/data_grid.cpp \
        $${US_BASE}/date_interval.cpp \
        $${US_BASE}/exception.cpp \
        $${US_BASE}/file_location_info.cpp \
        $${US_BASE}/file_locations.cpp \
        $${US_BASE}/identifier.cpp \
        $${US_BASE}/integrator.cpp \
        $${US_BASE}/minimiser.cpp \
        $${US_BASE}/model.cpp \
        $${US_BASE}/moments.cpp \
        $${US_BASE}/named_object.cpp \
        $${US_BASE}/name_list.cpp \
        $${US_BASE}/object_pool.cpp \
        $${US_BASE}/output.cpp \
        $${US_BASE}/product_base.cpp \
        $${US_BASE}/variable_base.cpp \
        $${US_BASE}/random.cpp \
        $${US_BASE}/root_solver.cpp \
        $${US_BASE}/ustime.cpp \
        $${US_BASE}/trace.cpp \
        $${US_BASE}/utilities.cpp \
        $${US_BASE}/version.cpp
