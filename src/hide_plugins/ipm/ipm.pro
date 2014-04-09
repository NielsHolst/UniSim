include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = ipm_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include ($${US_GSL}/gsl.pri)
include ($${US_QWT}/qwt.pri) 

HEADERS += \
	../unisim/physiological_time.h \
	brierei_time.h \
	briereii_time.h \	
	cuadratic_long_time.h \
	demand.h \
	even_odd.h \
	fecundity.h \
    fecundity5_time.h \
	g1_loss_rate.h \	
	scheduled_g1_loss_rate.h \
	g2_loss_rate.h \
	ipm_factory.h \
	lactin2_time.h \
	lamb_time.h \
	linear_time.h \
    loganb_time.h \

SOURCES += \
	../unisim/physiological_time.cpp \
	brierei_time.cpp \
	briereii_time.cpp \	
	cuadratic_long_time.cpp \
	demand.cpp \
	even_odd.cpp \
	fecundity.cpp \
    fecundity5_time.cpp \
	g1_loss_rate.cpp \	
	scheduled_g1_loss_rate.cpp \
	g2_loss_rate.cpp \
	ipm_factory.cpp \
	lactin2_time.cpp \
	lamb_time.cpp \
	linear_time.cpp \
    loganb_time.cpp	\
