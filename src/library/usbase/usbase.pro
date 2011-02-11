include(../../config.pri)

TEMPLATE = lib
CONFIG += dll

DESTDIR = $${US_BASE}/..
TARGET = $${US_BASE_LIB_NAME}

# Source code
HEADERS += \
        $${US_BASE}/clock.h \
        $${US_BASE}/component.h \
        $${US_BASE}/dataset.h \
        $${US_BASE}/exception.h \
        $${US_BASE}/file_location_info.h \
        $${US_BASE}/file_locations.h \
        $${US_BASE}/identifier.h \
        $${US_BASE}/integrator.h \
        $${US_BASE}/integrator_maker_plug_in.h \
        $${US_BASE}/object_pool.h \
		$${US_BASE}/output.h \
		$${US_BASE}/output_maker_plug_in.h \
        $${US_BASE}/output_data.h \
        $${US_BASE}/output_parameter.h \
        $${US_BASE}/output_result.h \
        $${US_BASE}/output_variable.h \
		$${US_BASE}/model.h \
		$${US_BASE}/model_maker_plug_in.h \
        $${US_BASE}/named_object.h \
        $${US_BASE}/parameter.h \
        $${US_BASE}/parameter_base.h \
        $${US_BASE}/pull_variable.h \
        $${US_BASE}/pull_variable_base.h \
        $${US_BASE}/push_variable.h \
        $${US_BASE}/push_variable_base.h \
        $${US_BASE}/time.h \
        $${US_BASE}/utilities.h \
        $${US_BASE}/version.h \
        $${US_BASE}/weather_file.h

SOURCES += \
        $${US_BASE}/clock.cpp \
        $${US_BASE}/component.cpp \
        $${US_BASE}/dataset.cpp \
        $${US_BASE}/exception.cpp \
        $${US_BASE}/file_location_info.cpp \
        $${US_BASE}/file_locations.cpp \
        $${US_BASE}/identifier.cpp \
        $${US_BASE}/integrator.cpp \
        $${US_BASE}/model.cpp \
        $${US_BASE}/named_object.cpp \
        $${US_BASE}/object_pool.cpp \
        $${US_BASE}/output.cpp \
        $${US_BASE}/output_data.cpp \
        $${US_BASE}/output_parameter.cpp \
        $${US_BASE}/output_result.cpp \
		$${US_BASE}/output_variable.cpp \
        $${US_BASE}/parameter_base.cpp \
        $${US_BASE}/pull_variable_base.cpp \
        $${US_BASE}/push_variable_base.cpp \
        $${US_BASE}/time.cpp \
        $${US_BASE}/utilities.cpp \
        $${US_BASE}/version.cpp \
        $${US_BASE}/weather_file.cpp

