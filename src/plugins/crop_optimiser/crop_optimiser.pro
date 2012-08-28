include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = crop_optimiser_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	crop_optimiser_factory.h \
	farm.h \
	field.h \
    crop.h \
    livestock.h \
    landscape.h

SOURCES += \
	crop_optimiser_factory.cpp \
	farm.cpp \
	field.cpp \
    crop.cpp \
    livestock.cpp \
    landscape.cpp











