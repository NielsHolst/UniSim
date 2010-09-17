@echo off
for /D %%i in (..\..\applications\*build-desktop) do rd %%i /S /Q
for /D %%i in (..\..\plugins\*build-desktop) do rd %%i /S /Q
