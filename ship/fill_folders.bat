set UNISIM_SHIP=C:\data\QDev\UniSim\ship
set GV_NAME=Graphviz2.28
set SOURCE_GV="C:\Program Files\%GV_NAME%"
set QT_PLUGINS="C:\Qt\5.4\mingw491_32\plugins"
if not exist bin md bin
if not exist output md output
if not exist plugins md plugins
if not exist temp md temp

rem Clean target bin and set target bin variable
del /Q bin\*.*
set UNISIM_BIN=%UNISIM_SHIP%\bin

rem Copy Graphviz
set UNISIM_GV=%UNISIM_SHIP%\%GV_NAME%
xcopy %SOURCE_GV% %UNISIM_GV% /I /Y /S
del /Q /S %UNISIM_GV%\include
del /Q /S %UNISIM_GV%\lib\debug
del /Q /S %UNISIM_GV%\lib\release

rem Copy MS files
pushd \Windows\System32
copy atl.dll %UNISIM_BIN%
copy comctl32.dll %UNISIM_BIN%
copy mfc42u.dll %UNISIM_BIN%
copy msvcrt.dll %UNISIM_BIN%
copy oleaut32.dll %UNISIM_BIN%
popd

rem Copy exe and DLL files
copy ..\src\applications\unisim*.exe bin
copy ..\src\applications\usbase*.dll bin
copy ..\src\applications\usengine*.dll bin
copy ..\src\applications\qwt.dll bin
copy ..\vendor\gsl\bin\libgsl*.dll bin

rem copy MinGW libraries
pushd %QT_ROOT%
copy icu*.dll %UNISIM_BIN%
copy libwinpthread-1.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy libstd*.dll %UNISIM_BIN%
rem copy Qt libraries
copy qt5core.dll %UNISIM_BIN%
copy qt5gui.dll %UNISIM_BIN%
copy Qt5Network.dll %UNISIM_BIN%
copy Qt5PrintSupport.dll %UNISIM_BIN%
copy qt5test.dll %UNISIM_BIN%
copy Qt5Widgets.dll %UNISIM_BIN%
copy qt5xmlpatterns.dll %UNISIM_BIN%
popd
rem create Qt plugin folders
pushd %UNISIM_BIN%
md accessible 
md imageformats
md platforms
popd
rem copy Qt plugin folders
pushd %QT_PLUGINS%
copy imageformats %UNISIM_BIN%\imageformats
copy platforms %UNISIM_BIN%\platforms
popd
rem Remove debug versions of Qt plugins
pushd %UNISIM_BIN%
del /Q accessible\*d.dll
del /Q imageformats\*d.dll
del /Q platforms\*d.dll
popd
:end_copy_lib

rem Copy UniSim plugins
del /Q plugins\*.*
copy ..\src\plugins\*.dll plugins
pause