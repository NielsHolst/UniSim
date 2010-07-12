include(../../config.pri)

TEMPLATE	= lib
DESTDIR	= 	$${US_PLUGINS}
TARGET 	= unisim_integrators_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG		+= plugin

include ($${US_BASE}/base.pri)

HEADERS	+= 	\
        time_limited.h \
        time_step_limited.h \
        unisim_integrator_maker.h
			  	
SOURCES	+= 	\
        time_limited.cpp \
        time_step_limited.cpp \
        unisim_integrator_maker.cpp
