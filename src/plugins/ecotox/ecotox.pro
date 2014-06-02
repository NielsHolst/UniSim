include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ecotox_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    ecotox_factory.h \
    log_logistic.h \
    pollen.h \
    pollen_onset_date.h \
    pollen_onset_date_from_maize_obs.h \
    pollen_onset_date_from_trap_obs.h \
    publish.h

SOURCES += \
    ecotox_factory.cpp \
    log_logistic.cpp \
    pollen.cpp \
    pollen_onset_date.cpp \
    pollen_onset_date_from_maize_obs.cpp \
    pollen_onset_date_from_trap_obs.cpp \
    publish.cpp

