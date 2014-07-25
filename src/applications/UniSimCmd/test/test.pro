include(../../../config.pri)

TEMPLATE = app
DESTDIR = $${US_APPLICATIONS}
TARGET = test-unisimcmd-$${UNISIM_VERSION}$${DEBUG_SUFFIX}
CONFIG += console
QT += testlib

QMAKE_CXXFLAGS += -Wno-write-strings

# Load application icon
RC_FILE = ../../../resources/unisim.rc
 
# Load other resources
RESOURCES = ../../../resources/unisim.qrc

# Libraries used
include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include ($${US_TCLAP}/tclap.pri)

# Input
include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)

HEADERS += \
    ../args.h \
    test_args.h
    
SOURCES += \
    ../args.cpp \
    main.cpp \
    test_args.cpp
