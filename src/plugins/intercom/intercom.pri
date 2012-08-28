INTERCOM_PLUGIN = $${US_PLUGINS}/intercom


HEADERS += \
    $${US_PLUGINS}/unisim/calendar.h \
    $${INTERCOM_PLUGIN}/assimilation_max_given_temp.h \
    $${INTERCOM_PLUGIN}/area.h \
    $${INTERCOM_PLUGIN}/area_density.h \
    $${INTERCOM_PLUGIN}/area_density_even.h \
    $${INTERCOM_PLUGIN}/area_density_tapering.h \
    $${INTERCOM_PLUGIN}/area_density_topheavy.h \
    $${INTERCOM_PLUGIN}/area_density_symmetric.h \
    $${INTERCOM_PLUGIN}/community.h \
    $${INTERCOM_PLUGIN}/constants.h \
    $${INTERCOM_PLUGIN}/early_growth.h \
    $${INTERCOM_PLUGIN}/height.h \
    $${INTERCOM_PLUGIN}/intercom_factory.h \
    $${INTERCOM_PLUGIN}/instant_mortality.h \
    $${INTERCOM_PLUGIN}/light_use_efficiency_given_temp.h \
    $${INTERCOM_PLUGIN}/mass.h \
    $${INTERCOM_PLUGIN}/optimize_allocation.h \
    $${INTERCOM_PLUGIN}/organ.h \
    $${INTERCOM_PLUGIN}/partitioning.h \
    $${INTERCOM_PLUGIN}/partitioning_fixed.h \
    $${INTERCOM_PLUGIN}/partitioning_fixed_by_stage.h \
    $${INTERCOM_PLUGIN}/phenology.h \
    $${INTERCOM_PLUGIN}/plant.h \
    $${INTERCOM_PLUGIN}/specific_leaf_area.h \
    $${INTERCOM_PLUGIN}/weather.h

SOURCES += \
    $${US_PLUGINS}/unisim/calendar.cpp \
    $${INTERCOM_PLUGIN}/assimilation_max_given_temp.cpp \
    $${INTERCOM_PLUGIN}/area.cpp \
    $${INTERCOM_PLUGIN}/area_density.cpp \
    $${INTERCOM_PLUGIN}/area_density_even.cpp \
    $${INTERCOM_PLUGIN}/area_density_tapering.cpp \
    $${INTERCOM_PLUGIN}/area_density_topheavy.cpp \
    $${INTERCOM_PLUGIN}/area_density_symmetric.cpp \
    $${INTERCOM_PLUGIN}/community.cpp \
    $${INTERCOM_PLUGIN}/early_growth.cpp \
    $${INTERCOM_PLUGIN}/height.cpp \
    $${INTERCOM_PLUGIN}/intercom_factory.cpp \
    $${INTERCOM_PLUGIN}/instant_mortality.cpp \
    $${INTERCOM_PLUGIN}/light_use_efficiency_given_temp.cpp \
    $${INTERCOM_PLUGIN}/mass.cpp \
    $${INTERCOM_PLUGIN}/optimize_allocation.cpp \
    $${INTERCOM_PLUGIN}/organ.cpp \
    $${INTERCOM_PLUGIN}/partitioning.cpp \
    $${INTERCOM_PLUGIN}/partitioning_fixed.cpp \
    $${INTERCOM_PLUGIN}/partitioning_fixed_by_stage.cpp \
    $${INTERCOM_PLUGIN}/phenology.cpp \
    $${INTERCOM_PLUGIN}/plant.cpp \
    $${INTERCOM_PLUGIN}/specific_leaf_area.cpp \
    $${INTERCOM_PLUGIN}/weather.cpp
