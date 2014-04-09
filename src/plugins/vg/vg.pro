include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = vg_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    adjustable_setpoint.h \
    air_infiltration.h \
    blackout_screen_controller.h \
#    boundary_layer_resistance.h \
    construction_cover.h \
    construction_geometry.h \
    construction_ventilation.h \
    control_element.h \
    control_element_asym.h \
    control_element_base.h \
    controlled_switch_setpoint.h \
    day_or_night_base.h \
    day_or_night_by_light.h \
    day_or_night_by_sun.h \
#    cover_dew.h \
#    cover_temperature.h \
#    cover_vapour_flux.h \
#    crop.h \
#    crop_layer.h \
#    crop_radiation_absorption.h \
    directed_control.h \
    energy_screen_balance.h \
    energy_screen_controller.h \
    general.h \
#    greenhouse_energetics.h \
#    heating_demand.h \
    heat_pipe.h \
    heating_flow_controller.h \
    heating_temperature_controller.h \
    high_light_switch.h \
    humidity_screen_balance.h \
    humidity_setpoints.h \
    indoors_humidity.h \
    indoors_radiation.h \
    indoors_temperature.h \
    indoors_ventilation.h \
#    lamp_attributes.h \
    light_controller.h \
    low_light_switch.h \
#    microclimate_humidity.h \
#    microclimate_temperature.h \
#    pid_control.h \
#    pipes.h \
    outdoors.h \
    proportional_control.h \
    screen.h \
    screen_controller.h \
#    screen_dew.h \
#    screen_temperature.h \
#    screen_transmission.h \
    shade_screen_controller.h \
    surface.h \
#    stomatal_resistance.h \
    switch_setpoint.h \
    switch_setpoint_collection.h \
    time_switch.h \
    variable_collection.h \
    ventilation_by_temperature_diff.h \
    ventilation_controller.h \
#    ventilation_latent_energy_balance.h \
    ventilation_max.h \
    vents_ventilation.h \
    vg_factory.h

SOURCES += \
    adjustable_setpoint.cpp \
    air_infiltration.cpp \
    blackout_screen_controller.cpp \
#    boundary_layer_resistance.cpp \
    construction_cover.cpp \
    construction_geometry.cpp \
    construction_ventilation.cpp \
    control_element.cpp \
    control_element_asym.cpp \
    control_element_base.cpp \
    controlled_switch_setpoint.cpp \
    day_or_night_base.cpp \
    day_or_night_by_light.cpp \
    day_or_night_by_sun.cpp \
#    cover_dew.cpp \
#    cover_temperature.cpp \
#    cover_vapour_flux.cpp \
#    crop.cpp \
#    crop_layer.cpp \
#    crop_radiation_absorption.cpp \
    directed_control.cpp \
    energy_screen_balance.cpp \
    energy_screen_controller.cpp \
    general.cpp \
#    greenhouse_energetics.cpp \
#    heating_demand.cpp \
    heat_pipe.cpp \
    heating_flow_controller.cpp \
    heating_temperature_controller.cpp \
    high_light_switch.cpp \
    indoors_ventilation.cpp \
    humidity_screen_balance.cpp \
    humidity_setpoints.cpp \
    indoors_humidity.cpp \
    indoors_radiation.cpp \
    indoors_temperature.cpp \
#    lamp_attributes.cpp \
    light_controller.cpp \
    low_light_switch.cpp \
#    microclimate_humidity.cpp \
#    microclimate_temperature.cpp \
#    pid_control.cpp \
#    pipes.cpp \
    outdoors.cpp \
    proportional_control.cpp \
    screen.cpp \
    screen_controller.cpp \
#    screen_dew.cpp \
#    screen_temperature.cpp \
#    screen_transmission.cpp \
    shade_screen_controller.cpp \
    surface.cpp \
#    stomatal_resistance.cpp \
    switch_setpoint.cpp \
    switch_setpoint_collection.cpp \
    time_switch.cpp \
    variable_collection.cpp \
    ventilation_by_temperature_diff.cpp \
    ventilation_controller.cpp \
#    ventilation_latent_energy_balance.cpp \
    ventilation_max.cpp \
    vents_ventilation.cpp \
    vg_factory.cpp
