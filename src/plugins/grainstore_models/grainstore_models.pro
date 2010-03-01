include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = grainstore_models$${SUFFIX_STR}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    ../standard_models/stage.h \
	grainstore_model_maker.h \
    larva_time.h \
	maize.h \
	prostephanus.h \
	pt_development.h \
	pt_emigration.h \
    pt_immigration.h \
    pt_reproduction.h \
    weather.h

SOURCES += \
    ../standard_models/stage.cpp \
    grainstore_model_maker.cpp \
    larva_time.cpp \
    maize.cpp \
	prostephanus.cpp \
	pt_development.cpp \
	pt_emigration.cpp \
    pt_immigration.cpp \
    pt_reproduction.cpp \
    weather.cpp

