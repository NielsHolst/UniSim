set UNISIM_SHIP=C:\data\QDev\UniSim\ship_cmd\UniSimCmd

rem create folders
md UniSimCmd
md UniSimCmd\bin

rem Set and clean target bin variable
set UNISIM_BIN=%UNISIM_SHIP%\bin
del /Q %UNISIM_BIN%\*.*
del /Q %UNISIM_BIN%\plugins\*.*

rem Copy MS files
pushd \Windows\System32
copy atl.dll %UNISIM_BIN%
copy comctl32.dll %UNISIM_BIN%
copy mfc42u.dll %UNISIM_BIN%
copy msvcrt.dll %UNISIM_BIN%
copy oleaut32.dll %UNISIM_BIN%
popd

rem Copy exe and DLL files
copy ..\src\applications\unisimcmd.exe %UNISIM_BIN%
copy ..\src\applications\usbase.dll %UNISIM_BIN%
copy ..\src\applications\usengine.dll %UNISIM_BIN%
copy ..\vendor\gsl\bin\libgsl*.dll %UNISIM_BIN%
copy ..\vendor\qwt_qt5\lib\qwt.dll %UNISIM_BIN%

rem copy MinGW libraries
pushd %QT_ROOT%
copy icu*.dll %UNISIM_BIN%
copy libwinpthread-1.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy libstd~1.dll %UNISIM_BIN%
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
md plugins
popd
rem copy Qt plugin folders
pushd %QT_ROOT%
cd ..\plugins
copy accessible %UNISIM_BIN%\accessible
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
copy ..\src\plugins\vg*.dll %UNISIM_BIN%\plugins
copy ..\src\plugins\unisim*.dll %UNISIM_BIN%\plugins
set UNISIM_BIN=
pause