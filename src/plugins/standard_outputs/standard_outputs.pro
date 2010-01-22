#
# Make standard_outputs plugin library
#
include($$(UNISIM_ROOT)/src/config.pri)

TEMPLATE	= lib
DESTDIR	= 	$${US_PLUGINS}
TARGET 	= standard_outputs$${SUFFIX_STR}
INCLUDEPATH += $${US_QWT}/include

CONFIG		+= plugin

LIBS += -L$${US_QWT}/lib \
    -lqwt$${SUFFIX_STR}5


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
