include(../../config.pri)

TEMPLATE	= lib
DESTDIR	= 	$${US_PLUGINS}
TARGET 	= standard_outputs_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
INCLUDEPATH += $${US_QWT}/include

CONFIG		+= plugin

LIBS += -L$${US_QWT}/lib \
    -lqwt$${DEBUG_SUFFIX}5


include ($${US_BASE}/base.pri)

HEADERS	+= 	\
        $${US_ENGINE}/main_window_interface.h \
        $${US_ENGINE}/plot_widget.h \
        output_plot.h \
        output_table.h \
        standard_output_maker.h
			  	
SOURCES	+= 	\
        $${US_ENGINE}/plot_widget.cpp \
        output_plot.cpp \
        output_table.cpp \
        standard_output_maker.cpp
