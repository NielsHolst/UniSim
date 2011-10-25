@echo off
for /D %%i in (..\..\applications\*build-desktop*) do rd %%i /S /Q
for /D %%i in (..\..\applications\unisim\*build-desktop*) do rd %%i /S /Q
for /D %%i in (..\..\library\*build-desktop*) do rd %%i /S /Q
for /D %%i in (..\..\plugins\*build-desktop*) do rd %%i /S /Q
