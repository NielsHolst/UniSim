include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = intercom_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    $${US_ENGINE}/simulation.h \
    ../unisim_models/calendar.h \
    assimilation_max_given_temp.h \
    assimilation_max_given_temp_sla.h \
    intercom_model_maker.h \
    area.h \
    canopy_layer.h \
    community.h \
    constants.h \
    height.h \
    leaf_stem_ratio.h \
    light_components.h \
    light_use_efficiency_given_temp.h \
    organ.h \
    photosynthesis.h \
    photosynthetic_rate.h \
    plant.h \
    specific_leaf_area.h \
    time_slice.h \
    weather.h

SOURCES += \
    $${US_ENGINE}/simulation.cpp \
    ../unisim_models/calendar.cpp \
    assimilation_max_given_temp.cpp \
    assimilation_max_given_temp_sla.cpp \
    intercom_model_maker.cpp \
    area.cpp \
    canopy_layer.cpp \
    community.cpp \
    height.cpp \
    leaf_stem_ratio.cpp \
    light_components.cpp	\
    light_use_efficiency_given_temp.cpp \
    organ.cpp \
    photosynthesis.cpp \
    photosynthetic_rate.cpp \
    plant.cpp \
    specific_leaf_area.cpp \
    time_slice.cpp \
    weather.cpp

