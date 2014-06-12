include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = insecticide_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    application.h \
    compartment.h \
    insecticide_factory.h \
    publish.h \
    substance.h \
    HazardQuotient.h \
    VolatilizationPlantLeaf.h \
    WindDrift.h \
    WindDriftTOT.h \
    leafvolatilization.h \
    leafuptake.h \
    leafphotodegradation.h

SOURCES += \
    application.cpp \
    compartment.cpp \
    insecticide_factory.cpp \
    publish.cpp \
    substance.cpp \
    HazardQuotient.cpp \
    VolatilizationPlantLeaf.cpp \
    WindDrift.cpp \
    WindDriftTOT.cpp \
    leafvolatilization.cpp \
    leafuptake.cpp \
    leafphotodegradation.cpp
