include(../../config.pri)

TEMPLATE = app
DESTDIR = $${US_APPLICATIONS}
TARGET = UniSimCmd
CONFIG += console


# Load application icon
RC_FILE = ../../resources/unisim.rc

# Load other resources
RESOURCES = ../../resources/unisim.qrc

# Libraries used
include ($${US_BASE}/base.pri)
include ($${US_ENGINE}/engine.pri)
include ($${US_QWT}/qwt.pri)
include ($${US_GSL}/gsl.pri)
include ($${US_TCLAP}/tclap.pri)

HEADERS += \
    args.h \
    file_locations_args.h \
    main_window_stub.h

SOURCES += \
    args.cpp \
    file_locations_args.cpp \
    main.cpp
