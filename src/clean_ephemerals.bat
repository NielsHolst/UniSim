@echo off
call cmd\clean_proj applications\unisim
call cmd\clean_proj applications\unisim\test
call cmd\clean_proj plugins\ambrosia_models
call cmd\clean_proj plugins\beehive_models
call cmd\clean_proj plugins\conductance_models
call cmd\clean_proj plugins\ess2009_models
call cmd\clean_proj plugins\grainstore_models
call cmd\clean_proj plugins\intercom_models
call cmd\clean_proj plugins\standard_models
call cmd\clean_proj plugins\standard_controllers
call cmd\clean_proj plugins\standard_outputs
call cmd\clean_proj plugins\test_models
if exist makefile.* del makefile.* /Q
if exist plugins\makefile.* del plugins\makefile.* /Q
@echo on
@echo *
@echo * UniSim project cleaned for all intermediate files *
@echo *
