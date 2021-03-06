include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = insecticide_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    application.h \
    crop.h \
    primarydistributionleaf.h \
    insecticide_factory.h \
    publish.h \
    topsoildegradation.h \
    riskindex.h \
    leafvolatilization.h \    
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
    leaffinaldosevolatilized.h \
    leaffinaldoseuptake.h \
    leafdosephotodegraded.h \
    leaffinaldosephotodegraded.h \
    topsoildoseinsoilsolidsairandwater.h \
    topsoildegradationfinal.h \
    topsoilvolatilizationfinal.h \
    doseavailableforleachingrunoffandmacroporeflow.h \
    doseavailableforleachingandmacroporeflow.h \
    doseavailableforleaching.h \
    totalcorrectionfactor.h \
    topsoildoseinsoilairandwater.h \
    soildegradationrateconstant.h \
    retardationtime.h \
    retardationfactor.h \
    porewatervelocity.h \
    pHcorrectedsorption.h \
    leafdosevolatilized.h \
    leafdoseuptaken.h \
    idealretardationtime.h \
    horizonthickness.h \
    fractionleachatereachingnextlayer.h \
    fractiondrainagedepth.h \
    filtervelocity.h \
    fieldcapacity.h \
    EaEp.h \
    drainagepresent.h \
    drainagelosses.h \
    depthcorrectionperhorizon.h \
    depthcorrectionat60to100cm.h \
    depthcorrectionat30to60cm.h \
    depthcorrectionat0to30cm.h \
    DemLaxFrnl.h \
    biodegradationrateperhorizon.h \
    bioavailabilitycorrection.h \
    apparentdiffusioncoefficient.h \
    apparentdiffusionanddispersioncoefficient.h \
    annualaveragesoiltemperaturefactor.h \
    frogmortality.h \
    frogpopulation.h \
    pesticideinducedmortalityrate.h \
    taverage.h \
    frogmalformation.h \
    frogpopulationmalformed.h \
    stagesurvivaldensity.h

SOURCES += \
    application.cpp \
    crop.cpp \
    primarydistributionleaf.cpp \
    insecticide_factory.cpp \
    publish.cpp \
    topsoildegradation.cpp \
    riskindex.cpp \
    leafvolatilization.cpp \    
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
    leaffinaldosevolatilized.cpp \
    leaffinaldoseuptake.cpp \
    leafdosephotodegraded.cpp \
    leaffinaldosephotodegraded.cpp \
    topsoildoseinsoilsolidsairandwater.cpp \
    topsoildegradationfinal.cpp \
    topsoilvolatilizationfinal.cpp \
    doseavailableforleachingrunoffandmacroporeflow.cpp \
    doseavailableforleachingandmacroporeflow.cpp \
    doseavailableforleaching.cpp \
    totalcorrectionfactor.cpp \
    topsoildoseinsoilairandwater.cpp \
    soildegradationrateconstant.cpp \
    retardationtime.cpp \
    retardationfactor.cpp \
    porewatervelocity.cpp \
    pHcorrectedsorption.cpp \
    leafdosevolatilized.cpp \
    leafdoseuptaken.cpp \
    idealretardationtime.cpp \
    horizonthickness.cpp \
    fractionleachatereachingnextlayer.cpp \
    fractiondrainagedepth.cpp \
    filtervelocity.cpp \
    fieldcapacity.cpp \
    EaEp.cpp \
    drainagepresent.cpp \
    drainagelosses.cpp \
    depthcorrectionperhorizon.cpp \
    depthcorrectionat60to100cm.cpp \
    depthcorrectionat30to60cm.cpp \
    depthcorrectionat0to30cm.cpp \
    DemLaxFrnl.cpp \
    biodegradationrateperhorizon.cpp \
    bioavailabilitycorrection.cpp \
    apparentdiffusioncoefficient.cpp \
    apparentdiffusionanddispersioncoefficient.cpp \
    annualaveragesoiltemperaturefactor.cpp \
    frogmortality.cpp \
    frogpopulation.cpp \
    pesticideinducedmortalityrate.cpp \
    taverage.cpp \
    frogmalformation.cpp \
    frogpopulationmalformed.cpp \
    stagesurvivaldensity.cpp
