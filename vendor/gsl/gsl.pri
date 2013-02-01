# Enable use of GSL library
win32 {
    # On windows use binary found in vendor/gsl folder
    INCLUDEPATH += $${US_GSL}
    LIBS += -L$${US_GSL}/bin -llibgsl-0
}
unix {
    # On Unix find it in general library folder
    # The developer must have installed it there
    INCLUDEPATH += /usr/include
    LIBS += -L/usr/lib -lgsl -lgslcblas -lm
}
