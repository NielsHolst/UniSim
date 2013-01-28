# Enable use of Qwt library
win32 {
    # On windows use binary found in vendor/gsl folder
    INCLUDEPATH += $${US_QWT}/include
    LIBS += -L$${US_QWT}/lib -lqwt$${DEBUG_SUFFIX}
}
unix {
    # On Unix find it in general library folder
    # The developer must have installed it there
    INCLUDEPATH += /usr/include/qwt
    LIBS += -L/usr/lib -lqwt
}
