#
# Configuration of make process (can be edited)
#

UNISIM_VERSION      = 1_15

CONFIG += debug							# Activate this to build debug version
#CONFIG += release 						# Or, this to build release version
#CONFIG += debug_and_release build_all 	# Or, this to build both versions

CONFIG += unisim_development     # Activate this if targets are under development (usually, they are)
#CONFIG += unisim_deployment     # Or, this if targets will be used to produce an installation file

#
# Ensuing configuration (should not be edited)
#

include(unisim_root.pri)

CONFIG(debug, debug|release) {
    DEBUG_SUFFIX = d
}
else {
    DEBUG_SUFFIX =
}
    
DEFINES += UNISIM_VERSION

CONFIG(unisim_development) {
    DEFINES += UNISIM_DEVELOPMENT
}
else {
    DEFINES += UNISIM_DEPLOYMENT
}

QT += xml xmlpatterns

DEFINES += QT_FATAL_ASSERT

US_ROOT = $${UNISIM_ROOT}
US_BASE = $${UNISIM_ROOT}/src/library/usbase
US_ENGINE = $${UNISIM_ROOT}/src/library/usengine
US_APPLICATIONS = $${UNISIM_ROOT}/src/applications
US_PLUGINS = $${UNISIM_ROOT}/src/plugins
US_QWT = $${UNISIM_ROOT}/vendor/qwt

INCLUDEPATH += "$$(BOOST_ROOT)" "$${UNISIM_ROOT}/src/library" "$${US_PLUGINS}"
