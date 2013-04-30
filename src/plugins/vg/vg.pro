include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = vg_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    boundary_layer_resistance.h \
    crop_wetness.h \
    cover.h \
    indoors.h \
    indoors_humidity.h \
    indoors_radiation.h \
    indoors_radiation_absorption.h \
    indoors_temperature.h \
    general.h \
    greenhouse.h \
    greenhouse_energy.h \
    greenhouse_temperature.h \
    greenhouse_transmission.h \
    greenhouse_ventilation.h \
    lamp_base.h \
    lamp_hpsl_400.h \
    lamp_led.h \
    lamp_switch.h \
    outdoors.h \
    pipe.h \
    pipes.h \
    proportional_control.h \
    screen.h \
    screen_air_transmission.h \
    screen_energy_base.h \
    screen_energy_balance.h \
    screen_energy_light.h \
    screen_light_transmission.h \
    screen_shade.h \
    screen_temperature.h \
    screen_transmission.h \
    sp_co2.h \
    sp_heating.h \
    sp_humidity_base.h \
    sp_humidity_regular.h \
    sp_ventilation_opening.h \
    sp_ventilation_temperature.h \
    sky.h \
    stomatal_resistance.h \
    vg_factory.h

SOURCES += \
    boundary_layer_resistance.cpp \
    crop_wetness.cpp \
    cover.cpp \
    indoors.cpp \
    indoors_humidity.cpp \
    indoors_radiation.cpp \
    indoors_radiation_absorption.cpp \
    indoors_temperature.cpp \
    general.cpp \
    greenhouse.cpp \
    greenhouse_energy.cpp \
    greenhouse_temperature.cpp \
    greenhouse_transmission.cpp \
    greenhouse_ventilation.cpp \
    lamp_base.cpp \
    lamp_hpsl_400.cpp \
    lamp_led.cpp \
    lamp_switch.cpp \
    outdoors.cpp \
    pipe.cpp \
    pipes.cpp \
    proportional_control.cpp \
    screen.cpp \
    screen_air_transmission.cpp \
    screen_energy_base.cpp \
    screen_energy_balance.cpp \
    screen_energy_light.cpp \
    screen_light_transmission.cpp \
    screen_shade.cpp \
    screen_temperature.cpp \
    sp_co2.cpp \
    sp_heating.cpp \
    sp_humidity_base.cpp \
    sp_humidity_regular.cpp \
    sp_ventilation_opening.cpp \
    sp_ventilation_temperature.cpp \
    sky.cpp \
    stomatal_resistance.cpp \
    vg_factory.cpp
