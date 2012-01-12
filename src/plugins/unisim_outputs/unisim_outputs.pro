include(../../config.pri)

TEMPLATE	= lib
DESTDIR	= 	$${US_PLUGINS}
TARGET 	= unisim_outputs_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG		+= plugin

include ($${US_BASE}/base.pri)
include($${US_QWT}/qwt.pri)

HEADERS	+= 	\
    $${US_ENGINE}/main_window_interface.h \
    $${US_ENGINE}/plot_widget.h \
    plot.h \
    output_plot.h \
    output_table.h \
    unisim_output_maker.h
			  	
SOURCES	+= 	\
    $${US_ENGINE}/plot_widget.cpp \
    plot.cpp \
    output_plot.cpp \
    output_table.cpp \
    unisim_output_maker.cpp
