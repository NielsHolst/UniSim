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
    asymptotic_control.h \
    boundary_layer_resistance.h \
    construction_floor.h \
    construction_geometry.h \
    construction_ventilation.h \
    control_element.h \
    control_element_asym.h \
    control_element_base.h \
    cover.h \
    cover_condensation.h \
    cover_temperature.h \
    crop_growth.h\
    crop_transpiration.h\
#    cover_temperature.h \
#    cover_vapour_flux.h \
#    crop.h \
#    crop_layer.h \
#    crop_radiation_absorption.h \
    directed_control.h \
    energy_balance.h \
    energy_screen_balance.h \
    energy_flux_condensation.h \
    energy_flux_floor.h \
    energy_flux_surface.h \
    energy_flux_transpiration.h \
    energy_flux_ventilation.h \
    floor_temperature.h \
    general.h \
#    greenhouse_energetics.h \
#    heating_demand.h \
    heat_pipe.h \
    heating_temperature_controller.h \
    high_light_switch.h \
    humidity_setpoints.h \
    indoors_humidity.h \
    indoors_radiation.h \
    indoors_temperature.h \
    layer.h \
    layer_photosynthesis.h \
    leaf_temperature.h \
    leaf_light_response.h \
    leaf_transpiration.h \
#    lamp_attributes.h \
    light_controller.h \
    low_light_switch.h \
    pipes.h \
    outdoors.h \
    plant_transpiration.h \
    proportional_control.h \
    publish.h \
    radiation_absorbed.h \
    screen.h \
    screen_condensation.h \
    screen_controller.h \
#    screen_dew.h \
    screen_temperature.h \
    stomatal_resistance.h \
    switch_collection.h \
    switch_setpoint.h \
    switch_setpoint_collection.h \
    threshold_switch.h \
    threshold_value.h \
    time_switch.h \
    transmission_base.h \
    transmission_cover.h \
    transmission_screens.h \
    variable_collection.h \
    vapour_flux.h \
    ventilation_air.h \
    ventilation_vapour.h \
    ventilation_controller.h \
    vents_ventilation.h \
    vg_factory.h

SOURCES += \
    adjustable_setpoint.cpp \
    air_infiltration.cpp \
    asymptotic_control.cpp \
    boundary_layer_resistance.cpp \
    construction_floor.cpp \
    construction_geometry.cpp \
    construction_ventilation.cpp \
    control_element.cpp \
    control_element_asym.cpp \
    control_element_base.cpp \
    cover.cpp \
    cover_temperature.cpp \
    cover_condensation.cpp \
    crop_growth.cpp\
    crop_transpiration.cpp\
#    cover_vapour_flux.cpp \
#    crop.cpp \
#    crop_layer.cpp \
#    crop_radiation_absorption.cpp \
    directed_control.cpp \
    energy_balance.cpp \
    energy_flux_condensation.cpp \
    energy_flux_floor.cpp \
    energy_flux_surface.cpp \
    energy_flux_transpiration.cpp \
    energy_flux_ventilation.cpp \
    energy_screen_balance.cpp \
    floor_temperature.cpp \
    general.cpp \
#    greenhouse_energetics.cpp \
#    heating_demand.cpp \
    heat_pipe.cpp \
    heating_temperature_controller.cpp \
    high_light_switch.cpp \
    humidity_setpoints.cpp \
    indoors_humidity.cpp \
    indoors_radiation.cpp \
    indoors_temperature.cpp \
    layer.cpp \
    layer_photosynthesis.cpp \
    leaf_temperature.cpp \
    leaf_light_response.cpp \
    leaf_transpiration.cpp \
#    lamp_attributes.cpp \
    light_controller.cpp \
    low_light_switch.cpp \
    pipes.cpp \
    outdoors.cpp \
    plant_transpiration.cpp \
    proportional_control.cpp \
    publish.cpp \
    radiation_absorbed.cpp \
    screen.cpp \
    screen_condensation.cpp \
    screen_controller.cpp \
#    screen_dew.cpp \
    screen_temperature.cpp \
    stomatal_resistance.cpp \
    switch_collection.cpp \
    switch_setpoint.cpp \
    switch_setpoint_collection.cpp \
    threshold_switch.cpp \
    threshold_value.cpp \
    time_switch.cpp \
    transmission_base.cpp \
    transmission_cover.cpp \
    transmission_screens.cpp \
    vapour_flux.cpp \
    variable_collection.cpp \
    ventilation_air.cpp \
    ventilation_vapour.cpp \
    ventilation_controller.cpp \
    vents_ventilation.cpp \
    vg_factory.cpp
