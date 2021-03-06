include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = aphid_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    aphid_fecundity.h \
    aphid_factory.h \
    damagethreshold.h \
    publish.h \
    treatment.h \
    epizootics.h

SOURCES += \
    aphid_fecundity.cpp \
    aphid_factory.cpp \
    damagethreshold.cpp \
    publish.cpp \
    treatment.cpp \
    epizootics.cpp
