include(../../config.pri)

TEMPLATE	= lib
DESTDIR	= 	$${US_PLUGINS}
TARGET 	= standard_controllers$${SUFFIX_STR}

CONFIG		+= plugin

include ($${US_BASE}/base.pri)

HEADERS	+= 	\
		simple_controller.h \
	  	standard_controller_maker.h 
			  	
SOURCES	+= 	\
		simple_controller.cpp \
	  	standard_controller_maker.cpp
