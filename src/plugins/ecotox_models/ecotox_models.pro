include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ecotox_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	ecotox_model_maker.h \
    log_logistic.h \
	pollen.h

SOURCES += \
	ecotox_model_maker.cpp \
    log_logistic.cpp \
    pollen.cpp

