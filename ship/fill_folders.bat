set UNISIM_SHIP=C:\data\QDev\UniSim\ship
set GV_NAME=Graphviz2.28
set SOURCE_GV="C:\Program Files\%GV_NAME%"
if not exist bin md bin
if not exist output md output
if not exist plugins md plugins
if not exist temp md temp

rem Copy Graphviz
set UNISIM_GV=%UNISIM_SHIP%\%GV_NAME%
xcopy %SOURCE_GV% %UNISIM_GV% /I /Y /S
del /Q /S %UNISIM_GV%\include
del /Q /S %UNISIM_GV%\lib\debug
del /Q /S %UNISIM_GV%\lib\release

rem Copy exe and DLL files
del /Q bin\*.*del /Q bin\*.*
copy ..\src\applications\unisim*.exe bin
copy ..\src\applications\base*.dll bin
copy ..\src\applications\engine*.dll bin
copy ..\vendor\gsl\bin\libgsl*.dll bin
copy ..\vendor\qwt\lib\qwt.dll bin

rem Copy MinGW and Qt lib files
set UNISIM_BIN=%UNISIM_SHIP%\bin
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

pause

:newer_qt_versions
pushd %QT_ROOT%\mingw\bin
copy mingwm10.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy qtcore4.dll %UNISIM_BIN%
copy qtgui4.dll %UNISIM_BIN%
copy QtNetwork4.dll %UNISIM_BIN%
copy qtsvg4.dll %UNISIM_BIN%
copy qtxml4.dll %UNISIM_BIN%
copy qtxmlpatterns4.dll %UNISIM_BIN%
popd
:end_copy_lib

rem Copy UniSim plugins
del /Q plugins\*.*
copy ..\src\plugins\*.dll plugins
set UNISIM_BIN=
pause