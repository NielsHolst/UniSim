include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = insecticide_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    application.h \
    crop.h \
    primarydistributionleaf.h \
    insecticide_factory.h \
    publish.h \
    riskindex.h \
    leafvolatilization.h \
    winddrift.h \
    winddrifttotal.h \
    leafuptake.h \
    leafphotodegradation.h \
    topsoilvolatilization.h \
    topsoilrunoff.h \
    macroporeflow.h \
    organiccarbonwaterpartitioning.h \
    runoffamount.h \
    fractionofrainrunoff.h \
    fieldarea.h \
    soilporosity.h \
    zvalues.h \
    viscosityofwater.h \
    primarydistributiondrifttoair.h \
    primarydistributionsoil.h \
    leafoverallrateconstant.h \
    leafdoseleftonbeginningrainfall.h \
    leafsumofdoseremoved.h \
    leafdosephotodegraded.h \
    leafdosevolatilized.h \
    leafdoseuptaken.h \
    topsoildoseinsoilsolidsairandwater.h \
    topsoildoseinsoilairandwater.h \
    topsoildegradationfinal.h \
    topsoilvolatilizationfinal.h \
    doseavailableforleachingrunoffandmacroporeflow.h \
    doseavailableforleachingandmacroporeflow.h \
    doseavailableforleaching.h \
    horizonthickness.h \
    htc.h

SOURCES += \
    application.cpp \
    crop.cpp \
    primarydistributionleaf.cpp \
    insecticide_factory.cpp \
    publish.cpp \
    riskindex.cpp \
    leafvolatilization.cpp \
    winddrift.cpp \
    winddrifttotal.cpp \
    leafuptake.cpp \
    leafphotodegradation.cpp \
    topsoilvolatilization.cpp \
    topsoilrunoff.cpp \
    macroporeflow.cpp \
    organiccarbonwaterpartitioning.cpp \
    runoffamount.cpp \
    fractionofrainrunoff.cpp \
    fieldarea.cpp \
    soilporosity.cpp \
    zvalues.cpp \
    viscosityofwater.cpp \
    primarydistributiondrifttoair.cpp \
    primarydistributionsoil.cpp \
    leafoverallrateconstant.cpp \
    leafdoseleftonbeginningrainfall.cpp \
    leafsumofdoseremoved.cpp \
    leafdosephotodegraded.cpp \
    leafdosevolatilized.cpp \
    leafdoseuptaken.cpp \
    topsoildoseinsoilsolidsairandwater.cpp \
    topsoildoseinsoilairandwater.cpp \
    topsoildegradationfinal.cpp \
    topsoilvolatilizationfinal.cpp \
    doseavailableforleachingrunoffandmacroporeflow.cpp \
    doseavailableforleachingandmacroporeflow.cpp \
    doseavailableforleaching.cpp \
    horizonthickness.cpp \
    htc.cpp
