@rem ===================================================
@rem Set QT_ROOT to the location of your Qt installation
@rem ===================================================

@set QT_ROOT=C:\Qt\2009.05

@rem ===================================================

@set QTENV="%QT_ROOT%\bin\qtenv.bat"
@if exist %QTENV% goto :ok
@echo .
@echo Batch file (build_all.bat) could not find %QTENV%
@echo Edit build_all.bat to fix this and try again
@goto :end

:ok
@echo .
@echo *** You are about to build from %UNISIM_ROOT% ***
@echo *** with Boost library found in %BOOST_ROOT% ***
@echo *** using the Qt version in     %QT_ROOT%
@echo .
@echo Press Ctrl+C to break. Or:
pause

@call %QTENV%
@call clean_ephemerals.bat
@call clean_targets.bat
qmake build_all.pro
@echo .
@echo *** Finished qmake ***
@echo .

..\vendor\gnu_make\make
@echo .
@echo *** Finished make ***
@echo .

@call clean_ephemerals.bat
@echo .
@echo *** Finished build ***
@echo .

:end
pause