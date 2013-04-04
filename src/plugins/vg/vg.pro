include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = vg_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    crop_wetness.h \
    cover.h \
    indoors.h \
    indoors_humidity.h \
    indoors_temperature.h \
    general.h \
    greenhouse.h \
    greenhouse_transmission.h \
    indoors_radiation.h \
    lamp_base.h \
    lamp_hpsl_400.h \
    lamp_led.h \
    lamp_switch.h \
    outdoors.h \
    screen.h \
    screen_energy_base.h \
    screen_energy_balance.h \
    screen_energy_light.h \
    screen_shade.h \
    screen_temperature.h \
    screen_transmission.h \
    sp_heating.h \
    sp_heating_humidity_base.h \
    sp_heating_humidity_delta_x.h \
    sp_heating_humidity_rh.h \
    sp_humidity_base.h \
    sp_humidity_regular.h \
    sky.h \
    vg_factory.h

SOURCES += \
    crop_wetness.cpp \
    cover.cpp \
    indoors.cpp \
    indoors_humidity.cpp \
    indoors_temperature.cpp \
    general.cpp \
    greenhouse.cpp \
    greenhouse_transmission.cpp \
    indoors_radiation.cpp \
    lamp_base.cpp \
    lamp_hpsl_400.cpp \
    lamp_led.cpp \
    lamp_switch.cpp \
    outdoors.cpp \
    screen.cpp \
    screen_energy_base.cpp \
    screen_energy_balance.cpp \
    screen_energy_light.cpp \
    screen_shade.cpp \
    screen_temperature.cpp \
    screen_transmission.cpp \
    sp_heating.cpp \
    sp_heating_humidity_base.cpp \
    sp_heating_humidity_delta_x.cpp \
    sp_heating_humidity_rh.cpp \
    sp_humidity_base.cpp \
    sp_humidity_regular.cpp \
    sky.cpp \
    vg_factory.cpp
