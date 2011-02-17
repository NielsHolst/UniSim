include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = intercom_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    $${US_PLUGINS}/unisim_models/calendar.h \
    assimilation_max_given_temp.h \
    area.h \
    area_density.h \
    area_density_even.h \
    area_density_tapering.h \
    area_density_topheavy.h \
    area_density_symmetric.h \
    community.h \
    constants.h \
    early_growth.h \
    height.h \
    intercom_model_maker.h \
    leaf_stem_ratio.h \
    light_use_efficiency_given_temp.h \
    organ.h \
    partitioning_fixed.h \
    phenology.h \
    plant.h \
    specific_leaf_area.h \
    weather.h

SOURCES += \
    $${US_PLUGINS}/unisim_models/calendar.cpp \
    assimilation_max_given_temp.cpp \
    area.cpp \
    area_density.cpp \
    area_density_even.cpp \
    area_density_tapering.cpp \
    area_density_topheavy.cpp \
    area_density_symmetric.cpp \
    community.cpp \
    early_growth.cpp \
    height.cpp \
    intercom_model_maker.cpp \
    leaf_stem_ratio.cpp \
    light_use_efficiency_given_temp.cpp \
    organ.cpp \
    partitioning_fixed.cpp \
    phenology.cpp \
    plant.cpp \
    specific_leaf_area.cpp \
    weather.cpp


