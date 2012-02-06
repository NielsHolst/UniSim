include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = crop_optimiser_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	crop_optimiser_model_maker.h \
	farm.h \
	field.h \
    crop.h \
    livestock.h \
    landscape.h

SOURCES += \
	crop_optimiser_model_maker.cpp \
	farm.cpp \
	field.cpp \
    crop.cpp \
    livestock.cpp \
    landscape.cpp











