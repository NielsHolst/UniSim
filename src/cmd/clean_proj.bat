pushd %1
if exist debug rd debug /S /Q
if exist output rd output /S /Q
if exist release rd release /S /Q
if exist temp rd temp /S /Q
if exist makefile*.* del makefile*.* /Q
if exist makefile del makefile /Q
if exist object_script*.* del object_script*.* /Q
if exist *.user del *.user /Q
popd
