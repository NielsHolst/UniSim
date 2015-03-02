@set QTENV="%QT_ROOT%\mingw\bin\qtenv2.bat"
call %QTENV%
pushd ..\build\own_tools
update_settings
popd
pause