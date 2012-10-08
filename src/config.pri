#
# Configuration of make process (can be edited)
#

UNISIM_VERSION      = 1-39

CONFIG += unisim_development     # Activate this if targets are under development (usually, they are)
#CONFIG += unisim_deployment     # Or, this if targets will be used to produce an installation file

#
# Ensuing configuration (should not be edited)
#

QMAKE_LFLAGS +=  -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc

DEFINES += UNISIM_VERSION

CONFIG(unisim_development) {
    DEFINES += UNISIM_DEVELOPMENT
	CONFIG += debug	
}
else {
    DEFINES += UNISIM_DEPLOYMENT
	CONFIG += release	
}

CONFIG(debug, debug|release) {
    DEBUG_SUFFIX = d
}
else {
    DEBUG_SUFFIX =
}
    
QT += xml xmlpatterns

DEFINES += QT_FATAL_ASSERT

include(unisim_root.pri)
US_ROOT = $${UNISIM_ROOT}
US_BASE = $${UNISIM_ROOT}/src/library/usbase
US_ENGINE = $${UNISIM_ROOT}/src/library/usengine
US_APPLICATIONS = $${UNISIM_ROOT}/src/applications
US_PLUGINS = $${UNISIM_ROOT}/src/plugins
US_QWT = $${UNISIM_ROOT}/vendor/qwt
US_GSL = $${UNISIM_ROOT}/vendor/gsl
US_TCLAP = $${UNISIM_ROOT}/vendor/tclap

US_BASE_LIB_NAME = base_$${UNISIM_VERSION}$${DEBUG_SUFFIX}
US_ENGINE_LIB_NAME = engine_$${UNISIM_VERSION}$${DEBUG_SUFFIX}

INCLUDEPATH += "$$(BOOST_ROOT)" "$${UNISIM_ROOT}/src/library" "$${US_PLUGINS}"
