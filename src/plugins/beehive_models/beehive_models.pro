include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = beehive_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    beehive_model_maker.h \
    egg_survival.h \
    fecundity.h \
	insect1.h \
	insect2.h \
    insect4.h \
    insect6.h \
    insect7.h \
    insect8.h \
#    insect9.h \
    larva_survival.h \
	pollen.h \
#	pupa_survival.h \
    weather.h

SOURCES += \
    beehive_model_maker.cpp \
    egg_survival.cpp \
    fecundity.cpp \
	insect1.cpp \
    insect2.cpp \
    insect4.cpp \
    insect6.cpp \
    insect7.cpp \
    insect8.cpp \
 #   insect9.cpp \
    larva_survival.cpp \
	pollen.cpp \
#	pupa_survival.cpp \
    weather.cpp

