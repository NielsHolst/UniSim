pushd ..\own_tools\%1
qmake %1.pro
mingw32-make
popd
