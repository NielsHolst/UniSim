@rem Set environment for older Qt versions (up to 2010)
@set QTENV="%QT_ROOT%\bin\qtenv.bat"
@if exist %QTENV% goto :qt_root_ok

@rem Set environment for newer Qt versions (from 2011)
@set QTENV="%QT_ROOT%\mingw\bin\qtenv2.bat"
@if exist %QTENV% goto :qt_root_ok

@rem Set environment for newer Qt versions (from 2013)
@set QTENV="%QT_ROOT%\qtenv2.bat"
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
pushd .
call %QTENV%
popd

@rem Erase all exe and dll targets
call clean_targets
call clean_build_desktops

@rem Make own tools
call make_tool write_unisim_root
call make_tool write_clean_ephemerals
call make_tool write_plugins_project

@rem Generate batch and project files
..\own_tools\write_clean_ephemerals
..\own_tools\write_plugins_project
..\own_tools\write_unisim_root_app

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

mingw32-make

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

@rem Copy library files
call make_tool write_qwt_version
..\own_tools\write_qwt_version
pushd ..\..\applications
call get_libraries
popd
@echo .
@echo *** Copied library files ***
@echo .


@rem Update registry settings
call make_tool update_settings
..\own_tools\update_settings_app
@echo .
@echo *** Updated registry for UniSim ***
@echo .

:end
@pause