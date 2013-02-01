@rem Generated on 1 Feb 13 10:33
@echo off
call clean_folder /home/getachew/dev/unisim-1-42/src/build/own_tools/update_settings
call clean_folder /home/getachew/dev/unisim-1-42/src/build/own_tools/write_clean_ephemerals
call clean_folder /home/getachew/dev/unisim-1-42/src/build/own_tools/write_plugins_project
call clean_folder /home/getachew/dev/unisim-1-42/src/build/own_tools/write_unisim_root
call clean_folder /home/getachew/dev/unisim-1-42/src/applications/unisim
call clean_folder /home/getachew/dev/unisim-1-42/src/applications/unisim/test
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/ambrosia
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/awe
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/beehive
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/conductance
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/crop_optimiser
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/dynamic_photosynthesis
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/ecotox
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/intercom
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/lakeoxygen
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/microbial_community
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/mussel_bed
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/network_indicators
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/rvf
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/seed_emergence
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/strawberry
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/supply_demand
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/test
call clean_folder /home/getachew/dev/unisim-1-42/src/plugins/unisim
if exist /home/getachew/dev/unisim-1-42/src/makefile.* del /home/getachew/dev/unisim-1-42/src/makefile.* /Q
if exist /home/getachew/dev/unisim-1-42/src/plugins/makefile.* del /home/getachew/dev/unisim-1-42/src/plugins/makefile.* /Q
@echo on
@echo *
@echo * UniSim project cleaned for all intermediate files *
@echo *
