include(../../config.pri)

TEMPLATE = lib
DESTDIR	= $${US_PLUGINS}
TARGET = unisim_outputs_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

CONFIG += plugin

include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include ($${US_QWT}/qwt.pri)

HEADERS	+= 	\
#    $${US_ENGINE}/main_window_interface.h \
#    $${US_ENGINE}/plot_widget.h \
    output_crosstab.h \
    output_plot.h \
    output_table.h \
    output_table_base.h \
    plot.h \
    unisim_output_maker.h
			  	
SOURCES	+= 	\
#    $${US_ENGINE}/plot_widget.cpp \
    output_crosstab.cpp \
    output_plot.cpp \
    output_table.cpp \
    output_table_base.cpp \
    plot.cpp \
    unisim_output_maker.cpp
