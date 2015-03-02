include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = lipaphis_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include ($${US_GSL}/gsl.pri)
include ($${US_QWT}/qwt.pri)

HEADERS += \
    even_odd.h \
    aphid_fecundity.h \
    aphid_fecundity1.h \
    aphid_quality.h \
    leaf_nitrogen.h \
    leaf_nitrogen2.h \
    lipaphis_demand.h \
    lipaphis_demand_rate.h \
    logistic_growth.h \
    lipaphis_factory.h \
    predator_fecundity.h\
    publish.h \
    sex_ratio.h \
    single_immigration.h \
    wang_development.h

SOURCES += \
    even_odd.cpp \
    aphid_fecundity.cpp \
    aphid_fecundity1.cpp \
    aphid_quality.cpp \
    leaf_nitrogen.cpp \
    leaf_nitrogen2.cpp \
    lipaphis_demand.cpp \
    lipaphis_demand_rate.cpp \
    logistic_growth.cpp \
    lipaphis_factory.cpp \
    predator_fecundity.cpp\
    publish.cpp \
    sex_ratio.cpp \
    single_immigration.cpp \
    wang_development.cpp
