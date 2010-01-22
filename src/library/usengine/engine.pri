# Depends on include($$(UNISIM_ROOT)/config.pri)

# Enable use of Qwt library
INCLUDEPATH += $${US_QWT}/include
LIBS += -L$${US_QWT}/lib \
    -lqwt$${SUFFIX_STR}5

HEADERS += 	\
			$${US_ENGINE}/confirmation.h \
			$${US_ENGINE}/controller_maker.h \
			$${US_ENGINE}/file_locations_strict.h \
            $${US_ENGINE}/main_window_interface.h \
            $${US_ENGINE}/model_documentation_writer.h \
			$${US_ENGINE}/model_maker.h \
            $${US_ENGINE}/output_maker.h \
            $${US_ENGINE}/plot_widget.h \
            $${US_ENGINE}/prototype_maker.h \
			$${US_ENGINE}/simulation.h \
            $${US_ENGINE}/simulation_maker.h \
            $${US_ENGINE}/xml_elements_reader.h \
            $${US_ENGINE}/xml_expander.h \
            $${US_ENGINE}/xml_query_to_writer.h

SOURCES += 	\
			$${US_ENGINE}/confirmation.cpp \
			$${US_ENGINE}/controller_maker.cpp \
			$${US_ENGINE}/file_locations_strict.cpp \
			$${US_ENGINE}/model_documentation_writer.cpp \
            $${US_ENGINE}/model_maker.cpp \
            $${US_ENGINE}/output_maker.cpp \
            $${US_ENGINE}/plot_widget.cpp \
            $${US_ENGINE}/prototype_maker.cpp \
			$${US_ENGINE}/simulation.cpp \
            $${US_ENGINE}/simulation_maker.cpp \
            $${US_ENGINE}/xml_elements_reader.cpp \
            $${US_ENGINE}/xml_expander.cpp \
            $${US_ENGINE}/xml_query_to_writer.cpp
			
