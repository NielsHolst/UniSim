# Enable use of Qwt library
win32 {
    # On windows use binary found in vendor/qwt folder
    INCLUDEPATH += $${US_QWT}/include
    LIBS += -L$${US_QWT}/lib -lqwt$${DEBUG_SUFFIX}
}
unix {
    # On Unix the developer download and build Qwt
    INCLUDEPATH += /home/nho/dev/qwt-6.1-rc3/src         # /usr/include/qwt
    LIBS += -L/home/nho/dev/qwt-6.1-rc3/lib -lqwt        # -L/usr/lib -lqwt
}
