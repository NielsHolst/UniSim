INTERCOM_PLUGIN = $${US_PLUGINS}/intercom_models

HEADERS += \
    $${US_ENGINE}/simulation.h \
    $${US_PLUGINS}/unisim_models/calendar.h \
    $${INTERCOM_PLUGIN}/assimilation_max_given_temp.h \
    $${INTERCOM_PLUGIN}/assimilation_max_given_temp_sla.h \
    $${INTERCOM_PLUGIN}/area.h \
    $${INTERCOM_PLUGIN}/community.h \
    $${INTERCOM_PLUGIN}/constants.h \
    $${INTERCOM_PLUGIN}/height.h \
    $${INTERCOM_PLUGIN}/layer.h \
    $${INTERCOM_PLUGIN}/leaf_stem_ratio.h \
    $${INTERCOM_PLUGIN}/light_components.h \
    $${INTERCOM_PLUGIN}/light_use_efficiency_given_temp.h \
    $${INTERCOM_PLUGIN}/organ.h \
    $${INTERCOM_PLUGIN}/photosynthesis.h \
    $${INTERCOM_PLUGIN}/photosynthetic_rate.h \
    $${INTERCOM_PLUGIN}/plant.h \
    $${INTERCOM_PLUGIN}/plant_layers.h \
    $${INTERCOM_PLUGIN}/specific_leaf_area.h \
    $${INTERCOM_PLUGIN}/time_slice.h \
    $${INTERCOM_PLUGIN}/weather.h

SOURCES += \
    $${US_ENGINE}/simulation.cpp \
    $${US_PLUGINS}/unisim_models/calendar.cpp \
    $${INTERCOM_PLUGIN}/assimilation_max_given_temp.cpp \
    $${INTERCOM_PLUGIN}/assimilation_max_given_temp_sla.cpp \
    $${INTERCOM_PLUGIN}/area.cpp \
    $${INTERCOM_PLUGIN}/community.cpp \
    $${INTERCOM_PLUGIN}/height.cpp \
    $${INTERCOM_PLUGIN}/layer.cpp \
    $${INTERCOM_PLUGIN}/leaf_stem_ratio.cpp \
    $${INTERCOM_PLUGIN}/light_components.cpp	\
    $${INTERCOM_PLUGIN}/light_use_efficiency_given_temp.cpp \
    $${INTERCOM_PLUGIN}/organ.cpp \
    $${INTERCOM_PLUGIN}/photosynthesis.cpp \
    $${INTERCOM_PLUGIN}/photosynthetic_rate.cpp \
    $${INTERCOM_PLUGIN}/plant.cpp \
    $${INTERCOM_PLUGIN}/plant_layers.cpp \
    $${INTERCOM_PLUGIN}/specific_leaf_area.cpp \
    $${INTERCOM_PLUGIN}/time_slice.cpp \
    $${INTERCOM_PLUGIN}/weather.cpp

