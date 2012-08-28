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
    pollen_loss_constant.h \
    pollen_loss_sampled.h \
    pollen_onset_date.h \
    deposition_flush.h \
    deposition_monotonic_sampled.h \
    deposition_monotonic_weibull.h

SOURCES += \
    ecotox_factory.cpp \
    log_logistic.cpp \
    pollen.cpp \
    pollen_loss_constant.cpp \
    pollen_loss_sampled.cpp \
    pollen_onset_date.cpp \
    deposition_flush.cpp \
    deposition_monotonic_sampled.cpp \
    deposition_monotonic_weibull.cpp

