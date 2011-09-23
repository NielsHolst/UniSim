include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = SupplyDemand_models_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
	SupplyDemand_model_maker.h \
    acquisition.h \
    energy_budget.h \
    exposure.h \
	growth_demand.h \
	holometabola.h \
    life_table.h \
    life_stage.h \
    maize.h \
    reproduction_demand.h \
    reproduction_demand_per_capita.h \
    reproduction_demand_size_dependent.h \
	respiration_demand.h \
    split_on_shortening_days.h \
    weather.h

SOURCES += \
	SupplyDemand_model_maker.cpp \
    acquisition.cpp \
    energy_budget.cpp \
    exposure.cpp \
    growth_demand.cpp \
	holometabola.cpp \
    life_table.cpp \
    life_stage.cpp \
    maize.cpp \
    reproduction_demand.cpp \
    reproduction_demand_per_capita.cpp \
    reproduction_demand_size_dependent.cpp \
    respiration_demand.cpp \
    split_on_shortening_days.cpp \
    weather.cpp

