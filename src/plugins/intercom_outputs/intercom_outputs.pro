include(../../config.pri)

TEMPLATE	= lib
DESTDIR	= 	$${US_PLUGINS}
TARGET 	= intercom_outputs_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG		+= plugin

include ($${US_BASE}/base.pri)

HEADERS	+= 	\
        output_file.h \
        intercom_output_maker.h
			  	
SOURCES	+= 	\
        output_file.cpp \
        intercom_output_maker.cpp
