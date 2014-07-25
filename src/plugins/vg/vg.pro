include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = vg_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    air_infiltration.h \
    asymptotic_signal.h \
    base_signal.h \
    boundary_layer_resistance.h \
    co2_controller.h \
    construction_floor.h \
    construction_geometry.h \
    construction_ventilation.h \
    control_element.h \
    control_element_asym.h \
    control_element_base.h \
    cover.h \
    cover_condensation.h \
    cover_temperature.h \
    crop_biomass.h \
    crop_transpiration.h \
    date_time_signal.h \
    energy_balance.h \
    energy_screen_balance.h \
    energy_flux_condensation.h \
    energy_flux_floor.h \
    energy_flux_surface.h \
    energy_flux_transpiration.h \
    energy_flux_ventilation.h \
    fixed_signal.h \
    floor_temperature.h \
    general.h \
    heat_pipe.h \
    heating_temperature_controller.h \
    indoors_co2.h \
    indoors_humidity.h \
    indoors_radiation.h \
    indoors_temperature.h \
    indoors_ventilation.h \
#    lamp_attributes.h \
    layer.h \
    layer_photosynthesis.h \
    leaf_light_response.h \
    leaf_temperature.h \
    leaf_transpiration.h \
    light_controller.h \
    pipes.h \
    outdoors.h \
    proportional_signal.h \
    publish.h \
    radiation_absorbed.h \
    screen.h \
    screen_condensation.h \
    screen_temperature.h \
    stomatal_resistance.h \
    signal_collection.h \
    sliding_signal.h \
    threshold_signal.h \
    transmission_base.h \
    transmission_cover.h \
    transmission_screens.h \
    variable_collection.h \
    vapour_flux.h \
    ventilation_vapour.h \
    ventilation_controller.h \
    vents_ventilation.h \
    vg_factory.h

SOURCES += \
    air_infiltration.cpp \
    asymptotic_signal.cpp \
    base_signal.cpp \
    boundary_layer_resistance.cpp \
    co2_controller.cpp \
    construction_floor.cpp \
    construction_geometry.cpp \
    construction_ventilation.cpp \
    control_element.cpp \
    control_element_asym.cpp \
    control_element_base.cpp \
    cover.cpp \
    cover_temperature.cpp \
    cover_condensation.cpp \
    crop_biomass.cpp \
    crop_transpiration.cpp \
    date_time_signal.cpp \
    energy_balance.cpp \
    energy_flux_condensation.cpp \
    energy_flux_floor.cpp \
    energy_flux_surface.cpp \
    energy_flux_transpiration.cpp \
    energy_flux_ventilation.cpp \
    energy_screen_balance.cpp \
    fixed_signal.cpp \
    floor_temperature.cpp \
    general.cpp \
    heat_pipe.cpp \
    heating_temperature_controller.cpp \
    indoors_co2.cpp \
    indoors_humidity.cpp \
    indoors_radiation.cpp \
    indoors_temperature.cpp \
    indoors_ventilation.cpp \
#    lamp_attributes.cpp \
    layer.cpp \
    layer_photosynthesis.cpp \
    leaf_light_response.cpp \
    leaf_temperature.cpp \
    leaf_transpiration.cpp \
    light_controller.cpp \
    pipes.cpp \
    outdoors.cpp \
    proportional_signal.cpp \
    publish.cpp \
    radiation_absorbed.cpp \
    screen.cpp \
    screen_condensation.cpp \
    screen_temperature.cpp \
    stomatal_resistance.cpp \
    signal_collection.cpp \
    sliding_signal.cpp \
    threshold_signal.cpp \
    transmission_base.cpp \
    transmission_cover.cpp \
    transmission_screens.cpp \
    vapour_flux.cpp \
    ventilation_vapour.cpp \
    ventilation_controller.cpp \
    vents_ventilation.cpp \
    vg_factory.cpp
