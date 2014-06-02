include(../../config.pri)

TEMPLATE = lib
DESTDIR = $${US_PLUGINS}
TARGET = microbial_community_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += plugin

include ($${US_BASE}/base.pri)

HEADERS += \
    microbial_community_factory.h \
    enzymatic_reaction.h \
    population.h \
    first_order_reaction.h \
    zero_order_reaction.h \
    reaction.h \
    competition.h \
    community.h \
    general_effect.h \
    effect.h \
    random.h \
    competitive_inhibition.h

SOURCES += \
    microbial_community_factory.cpp \
    enzymatic_reaction.cpp \
    population.cpp \
    first_order_reaction.cpp \
    zero_order_reaction.cpp \
    reaction.cpp \
    competition.cpp \
    community.cpp \
    general_effect.cpp \
    effect.cpp \
    random.cpp \
    competitive_inhibition.cpp
