@echo off
if exist ..\..\applications\*.exe  del ..\..\applications\*.exe /Q
if exist ..\..\library\*.dll  del ..\..\library\*.dll /Q
if exist ..\..\library\*.a  del ..\..\library\*.a /Q
if exist ..\..\plugins\*.dll  del ..\..\plugins\*.dll /Q
if exist ..\..\plugins\*.a  del ..\..\plugins\*.a /Q
if exist ..\own_tools\*.exe del ..\own_tools\*.exe /Q
@echo on
@echo *
@echo * UniSim project cleaned for all target files *
@echo *
