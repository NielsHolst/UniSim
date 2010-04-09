include(../../config.pri)

TEMPLATE	= lib
DESTDIR	= 	$${US_PLUGINS}
TARGET 	= standard_integrators_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG		+= plugin

include ($${US_BASE}/base.pri)

HEADERS	+= 	\
		simple_integrator.h \
	  	standard_integrator_maker.h 
			  	
SOURCES	+= 	\
		simple_integrator.cpp \
	  	standard_integrator_maker.cpp
