include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = intercom_models$${SUFFIX_STR}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ../standard_models/calendar.h \
    assimilation_max_given_temp.h \
    assimilation_max_given_temp_sla.h \
    intercom_model_maker.h \
    area.h \
    community.h \
    constants.h \
    height.h \
    leaf_stem_ratio.h \
    light_components.h \
    light_use.h \
    organ.h \
	plant.h \
    specific_leaf_area.h \
    weather.h

SOURCES += \
    ../standard_models/calendar.cpp \
    assimilation_max_given_temp.cpp \
    assimilation_max_given_temp_sla.cpp \
    intercom_model_maker.cpp \
    area.cpp \
    community.cpp \
    height.cpp \
    leaf_stem_ratio.cpp \
    light_components.cpp	\
    organ.cpp \
    plant.cpp \
    specific_leaf_area.cpp \
    weather.cpp

