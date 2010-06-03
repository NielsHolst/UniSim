set UNISIM_BIN=C:\data\QDev\UniSim\ship\bin
del /Q bin\*.*
del /Q plugins\*.*
copy ..\src\applications\unisim*.exe bin
copy ..\vendor\qwt\lib\qwt5.dll bin
pushd %QT_ROOT%\qt\bin
copy mingwm10.dll %UNISIM_BIN%
copy qtcore4.dll %UNISIM_BIN%
copy qtgui4.dll %UNISIM_BIN%
copy qtxml4.dll %UNISIM_BIN%
copy qtxmlpatterns4.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy QtNetwork4.dll %UNISIM_BIN%
popd
copy ..\src\plugins\*.dll plugins
set UNISIM_BIN=
pause