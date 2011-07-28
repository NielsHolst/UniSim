@rem Set enviroment for older Qt versions (up to 2010)
@set QTENV="%QT_ROOT%\bin\qtenv.bat"
@if exist %QTENV% goto :qt_root_ok

@rem Set enviroment for newer Qt versions (from 2011)
@set QTENV="%QT_ROOT%\mingw\bin\qtenv2.bat"
@if exist %QTENV% goto :qt_root_ok

@echo .
@echo Batch file (build_all.bat) could not find %QTENV%
@echo Set the QT_ROOT environment variable correctly and try again
@echo .
@goto :end

:qt_root_ok
@echo .
@echo . You are about to build Universal Simulator
@echo . using Qt found in.............  %QT_ROOT%
@echo . with Boost library found in...  %BOOST_ROOT%
@echo .
@echo Press Ctrl+C to break. Or:
@pause

@rem Copy authors list to UniSim root
call copy_authors

@rem Prepare system path for compiling
call %QTENV%

@rem Erase all exe and dll targets
call clean_targets
call clean_build_desktops

@rem Make own tools
call make_tool write_unisim_root
call make_tool update_settings
call make_tool write_clean_ephemerals
call make_tool write_plugins_project

@rem Generate batch and project files
..\own_tools\write_clean_ephemerals
..\own_tools\write_plugins_project
..\own_tools\write_unisim_root

@rem Clean build folders
call clean_ephemerals
call clean_folder ..\..\library\usbase
call clean_folder ..\..\library\usengine

@rem Build UniSim
pushd ..\..
qmake build_all.pro
@echo .
@echo *** Finished qmake ***
@echo .

..\vendor\gnu_make\make
popd
@echo .
@echo *** Finished make ***
@echo .

call clean_ephemerals.bat
call clean_folder ..\..\library\usbase
call clean_folder ..\..\library\usengine
@echo .
@echo *** Finished build ***
@echo .

copy ..\..\library\*.dll ..\..\applications
@echo .
@echo *** Copied UniSim library files ***
@echo .


@rem Update registry settings
..\own_tools\update_settings
@echo .
@echo *** Updated registry for UniSim ***
@echo .

:end
@pause