include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = beehive_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ../standard_models/stage.h \
    beehive_model_maker.h \
	aethina.h \
	fecundity.h \
	insect1.h \
	insect2.h \
    insect3.h \
    insect4.h

SOURCES += \
    ../standard_models/stage.cpp \
    beehive_model_maker.cpp \
	aethina.cpp \
	fecundity.cpp \
	insect1.cpp \
    insect2.cpp \
    insect3.cpp \
    insect4.cpp

