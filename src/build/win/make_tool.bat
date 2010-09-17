pushd ..\own_tools\%1
qmake %1.pro
..\..\..\..\vendor\gnu_make\make
popd
