include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
#TARGET = vg_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
TARGET = vg
CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    air_flux_cooling_supply.h \
    air_flux_cooling_supply_max.h \
    air_flux_gravitation.h \
    air_flux_indoors.h \
    air_flux_infiltration.h \
    air_flux_outdoors.h \
    base_control_element.h \
    base_signal.h \
    boundary_layer_resistance.h \
    co2_controller.h \
    construction_geometry.h \
    cover.h \
    cover_condensation.h \
    crop.h \
    crop_mass.h \
    crop_growth.h \
    crop_lai.h \
    crop_yield.h \
    date_time_signal.h \
    energy_flux_air.h \
    energy_flux_base.h \
    energy_flux_condensation.h \
    energy_flux_cooling_demand.h \
    energy_flux_cooling_supply.h \
    energy_flux_cover.h \
    energy_flux_cover_sum.h \
    energy_flux_floor.h \
    energy_flux_growth_lights.h \
    energy_flux_heating_demand.h \
    energy_flux_heating_supply.h \
    energy_flux_horizontal.h \
    energy_flux_shelter.h \
    energy_flux_sum.h \
    energy_flux_transpiration.h \
    energy_screen_balance_signal.h \
    fixed_signal.h \
    fruit_crop_lai.h \
    fruit_crop_mass.h \
    fruit_factor.h \
    general.h \
    greenhouse_shelter.h \
    growth_light_base.h \
    growth_light.h \
    growth_lights.h \
    indoors_co2.h \
    indoors_bottom_light.h \
    indoors_humidity.h \
    indoors_temperature.h \
    indoors_top_light.h \
    layer.h \
    layer_photosynthesis.h \
    leaf_light_response.h \
    leaf_temperature.h \
    leaf_transpiration.h \
    growth_light_controller.h \
    outdoors.h \
    pid_control_element.h \
    proportional_signal.h \
    publish.h \
    radiation_absorbed.h \
    running_average.h \
    screen.h \
    screens.h \
    screen_condensation.h \
    screen_temperature.h \
    shelter.h \
    shelter_outputs.h \
    signal_collection.h \
    sliding_signal.h \
    stomatal_resistance.h \
    threshold_signal.h \
    variable_collection.h \
    vapour_flux_air.h \
    vapour_flux_base.h \
    vapour_flux_sum.h \
    vapour_flux_sum_base.h \
    vapour_flux_transpiration.h \
    ventilated_latent_heat_converter.h \
    ventilation_by_temp.h \
    ventilation_by_wind.h \
    ventilation_controller.h \
    vent.h \
    vents.h \
    vg_factory.h

SOURCES += \
    air_flux_cooling_supply.cpp \
    air_flux_cooling_supply_max.cpp \
    air_flux_gravitation.cpp \
    air_flux_indoors.cpp \
    air_flux_infiltration.cpp \
    air_flux_outdoors.cpp \
    base_control_element.cpp \
    base_signal.cpp \
    boundary_layer_resistance.cpp \
    co2_controller.cpp \
    construction_geometry.cpp \
    cover.cpp \
    cover_condensation.cpp \
    crop.cpp \
    crop_mass.cpp \
    crop_growth.cpp \
    crop_lai.cpp \
    crop_yield.cpp \
    date_time_signal.cpp \
    energy_flux_air.cpp \
    energy_flux_base.cpp \
    energy_flux_condensation.cpp \
    energy_flux_cooling_demand.cpp \
    energy_flux_cooling_supply.cpp \
    energy_flux_cover.cpp \
    energy_flux_cover_sum.cpp \
    energy_flux_floor.cpp \
    energy_flux_growth_lights.cpp \
    energy_flux_heating_demand.cpp \
    energy_flux_heating_supply.cpp \
    energy_flux_horizontal.cpp \
    energy_flux_shelter.cpp \
    energy_flux_sum.cpp \
    energy_flux_transpiration.cpp \
    energy_screen_balance_signal.cpp \
    fixed_signal.cpp \
    fruit_crop_lai.cpp \
    fruit_crop_mass.cpp \
    fruit_factor.cpp \
    general.cpp \
    greenhouse_shelter.cpp \
    growth_light_base.cpp \
    growth_light.cpp \
    growth_lights.cpp \
    indoors_co2.cpp \
    indoors_bottom_light.cpp \
    indoors_humidity.cpp \
    indoors_temperature.cpp \
    indoors_top_light.cpp \
    layer.cpp \
    layer_photosynthesis.cpp \
    leaf_light_response.cpp \
    leaf_temperature.cpp \
    leaf_transpiration.cpp \
    growth_light_controller.cpp \
    outdoors.cpp \
    pid_control_element.cpp \
    proportional_signal.cpp \
    publish.cpp \
    radiation_absorbed.cpp \
    running_average.cpp \
    screen.cpp \
    screens.cpp \
    screen_condensation.cpp \
    screen_temperature.cpp \
    shelter.cpp \
    shelter_outputs.cpp \
    signal_collection.cpp \
    sliding_signal.cpp \
    stomatal_resistance.cpp \
    threshold_signal.cpp \
    vapour_flux_air.cpp \
    vapour_flux_base.cpp \
    vapour_flux_sum.cpp \
    vapour_flux_sum_base.cpp \
    vapour_flux_transpiration.cpp \
    ventilated_latent_heat_converter.cpp \
    ventilation_by_temp.cpp \
    ventilation_by_wind.cpp \
    ventilation_controller.cpp \
    vent.cpp \
    vents.cpp \
    vg_factory.cpp
