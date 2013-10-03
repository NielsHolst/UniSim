include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = vg_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    air_infiltration.h \
    blackout_screen_controller.h \
    boundary_layer_resistance.h \
    control_element.h \
    control_element_asym.h \
    control_element_base.h \
    cover_dew.h \
    cover_temperature.h \
    cover_vapour_flux.h \
    crop.h \
    crop_layer.h \
    crop_radiation_absorption.h \
    directed_control.h \
    energy_screen_balance.h \
    energy_screen_controller.h \
    environment.h \
    general.h \
    greenhouse_construction.h \
    greenhouse_energetics.h \
    greenhouse_ventilation.h \
    heating_demand.h \
    humidity_setpoints.h \
    indoors_radiation.h \
    lamp_attributes.h \
    light_controller.h \
    microclimate_humidity.h \
    microclimate_temperature.h \
    pid_control.h \
    pipe.h \
    pipes.h \
    proportional_control.h \
    screen_dew.h \
    screen_temperature.h \
    screen_transmission.h \
    screens.h \
    shade_screen_controller.h \
    stomatal_resistance.h \
    temperature_setpoints.h \
    variable_collection.h \
    ventilation_by_temperature_diff.h \
    ventilation_controller.h \
    ventilation_latent_energy_balance.h \
    ventilation_max.h \
    vg_factory.h \
    windows_ventilation.h

SOURCES += \
    air_infiltration.cpp \
    blackout_screen_controller.cpp \
    boundary_layer_resistance.cpp \
    control_element.cpp \
    control_element_asym.cpp \
    control_element_base.cpp \
    cover_dew.cpp \
    cover_temperature.cpp \
    cover_vapour_flux.cpp \
    crop.cpp \
    crop_layer.cpp \
    crop_radiation_absorption.cpp \
    directed_control.cpp \
    energy_screen_balance.cpp \
    energy_screen_controller.cpp \
    environment.cpp \
    general.cpp \
    greenhouse_construction.cpp \
    greenhouse_energetics.cpp \
    greenhouse_ventilation.cpp \
    heating_demand.cpp \
    humidity_setpoints.cpp \
    indoors_radiation.cpp \
    lamp_attributes.cpp \
    light_controller.cpp \
    microclimate_humidity.cpp \
    microclimate_temperature.cpp \
    pid_control.cpp \
    pipe.cpp \
    pipes.cpp \
    proportional_control.cpp \
    screen_dew.cpp \
    screen_temperature.cpp \
    screen_transmission.cpp \
    screens.cpp \
    shade_screen_controller.cpp \
    stomatal_resistance.cpp \
    temperature_setpoints.cpp \
    variable_collection.cpp \
    ventilation_by_temperature_diff.cpp \
    ventilation_controller.cpp \
    ventilation_latent_energy_balance.cpp \
    ventilation_max.cpp \
    vg_factory.cpp \
    windows_ventilation.cpp
