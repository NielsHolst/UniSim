@echo off
if exist applications\*.exe  del  applications\*.exe /Q
if exist plugins\*.dll  del  plugins\*.dll /Q
if exist plugins\*.a  del  plugins\*.a /Q
@echo on
@echo *
@echo * UniSim project cleaned for all target files *
@echo *
