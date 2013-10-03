@echo off
for /D %%i in (..\..\applications\*build-*) do rd %%i /S /Q
for /D %%i in (..\..\applications\unisim\*build-*) do rd %%i /S /Q
for /D %%i in (..\..\library\*build-*) do rd %%i /S /Q
for /D %%i in (..\..\plugins\*build-*) do rd %%i /S /Q
for /D %%i in (..\own_tools\*build-*) do rd %%i /S /Q
