include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = intercom_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
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
    intercom_factory.h \
    instant_mortality.h \
    light_use_efficiency_given_temp.h \
    mass.h \
    optimize_allocation.h \
    organ.h \
    partitioning.h \
    partitioning_fixed.h \
    partitioning_fixed_by_stage.h \
    phenology.h \
    plant.h \
    specific_leaf_area.h \
    weather.h

SOURCES += \
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
    intercom_factory.cpp \
    instant_mortality.cpp \
    light_use_efficiency_given_temp.cpp \
    mass.cpp \
    optimize_allocation.cpp \
    organ.cpp \
    partitioning.cpp \
    partitioning_fixed.cpp \
    partitioning_fixed_by_stage.cpp \
    phenology.cpp \
    plant.cpp \
    specific_leaf_area.cpp \
    weather.cpp

