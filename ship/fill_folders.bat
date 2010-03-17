del /Q bin\*.*
del /Q plugins\*.*
copy ..\src\applications\unisim.exe bin
copy ..\vendor\qwt\lib\qwt5.dll bin
pushd \Qt\2009.05\qt\bin
copy mingwm10.dll \data\qdev\unisim\install_builder\bin
copy qtcore4.dll \data\qdev\unisim\install_builder\bin
copy qtgui4.dll \data\qdev\unisim\install_builder\bin
copy qtxml4.dll \data\qdev\unisim\install_builder\bin
copy qtxmlpatterns4.dll \data\qdev\unisim\install_builder\bin
popd
copy ..\src\plugins\*.dll plugins
