@echo off 
call clean_build_desktops
call clean_ephemerals
call clean_targets
del ..\..\..\doc\vg\*.* /Q /S > nul
del ..\..\..\src\applications\output\*.* /Q /S > nul
del ..\..\..\src\applications\temporary\*.* /Q /S > nul


