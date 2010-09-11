@set QTENV="%QT_ROOT%\bin\qtenv.bat"
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

@rem Generate batch and project files
..\own_tools\write_unisim_root
..\own_tools\write_clean_ephemerals
..\own_tools\write_plugins_project

call %QTENV%
call clean_ephemerals.bat
call clean_targets.bat
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
@echo .
@echo *** Finished build ***
@echo .

@rem Generate batch and project files
..\own_tools\update_settings
@echo .
@echo *** Updated registry for UniSim ***
@echo .

:end
@pause