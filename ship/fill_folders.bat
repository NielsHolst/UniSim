set UNISIM_SHIP=C:\data\QDev\UniSim\ship
set GV_NAME=Graphviz2.26.3
set SOURCE_GV="C:\Programmer\%GV_NAME%"

set UNISIM_BIN=%UNISIM_SHIP%\bin
set UNISIM_GV=%UNISIM_SHIP%\%GV_NAME%

xcopy %SOURCE_GV% %UNISIM_GV% /I /Y /S
del /Q /S %UNISIM_GV%\include
del /Q /S %UNISIM_GV%\lib\debug
del /Q /S %UNISIM_GV%\lib\release

del /Q bin\*.*del /Q bin\*.*
del /Q plugins\*.*
copy ..\src\applications\unisim*.exe bin
copy ..\src\applications\base*.dll bin
copy ..\src\applications\engine*.dll bin
copy ..\vendor\qwt\lib\qwt.dll bin

rem Copy MinGW and Qt lib files
if exist %QT_ROOT%\mingw\lib goto :newer_qt_versions
pushd %QT_ROOT%\qt\bin
copy mingwm10.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy qtcore4.dll %UNISIM_BIN%
copy qtgui4.dll %UNISIM_BIN%
copy QtNetwork4.dll %UNISIM_BIN%
copy qtxml4.dll %UNISIM_BIN%
copy qtxmlpatterns4.dll %UNISIM_BIN%
goto :end_copy_lib
popd

:newer_qt_versions
pushd %QT_ROOT%\mingw\bin
copy mingwm10.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
popd
pushd %QT_ROOT%\mingw\lib
copy qtcore4.dll %UNISIM_BIN%
copy qtgui4.dll %UNISIM_BIN%
copy QtNetwork4.dll %UNISIM_BIN%
copy qtsvg4.dll %UNISIM_BIN%
copy qtxml4.dll %UNISIM_BIN%
copy qtxmlpatterns4.dll %UNISIM_BIN%
popd
:end_copy_lib

rem Copy UniSim plugins
copy ..\src\plugins\*.dll plugins
set UNISIM_BIN=
pause