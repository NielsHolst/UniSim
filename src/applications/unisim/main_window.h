/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_MAIN_WINDOW_H
#define UniSim_MAIN_WINDOW_H

#include <QMainWindow>
#include <usbase/object_pool.h>
#include <usengine/main_window_interface.h>
#include "stored_widget.h"
#include "sub_window.h"

class QAction;
class QLabel;
class QMdiArea;
class QMenu;
class QPixmap;
class QScrollArea;
class QSettings;
class QwtPlot;
class LiveSimulation;
class LogSubWindow;

namespace UniSim {
    class Exception;
    class PlotWidget;
}

class MainWindow : public QMainWindow,
                   public UniSim::MainWindowInterface,
                   public StoredWidget
{
	Q_OBJECT

public:
    void createMenus();
    UniSim::PlotWidget* createPlotWidget(QString title);

private slots:
	void doFileOpen();
    void doFileReopen();
    void doFileReopenRun();
    void doFileLocations();
    void doFileClose();
	void doFileExit();
    void doEditModel();
    void doEditHelp();
    void doSimulationRun();
    void doToolsPrototyping();
    void doToolsGenerateDocs();
    void doHelpAbout();
    void doHelpHome();
    void standardizeSubWindows();
    void doWindowsSaveGraphics();

    void liveSimulatorStateChanged(int oldState, int newState);

	
private:
	// Singleton
	MainWindow();
	static MainWindow* _mainWindow;
	friend MainWindow* mainWindow();
	
	// Main window GUI
    QMenu *fileMenu, *editMenu, *simulationMenu, *toolsMenu, *viewMenu, *windowMenu, *helpMenu;
	QAction 
        *fileOpen, *fileReopen, *fileReopenRun, *fileEdit, *fileClose, *fileLocations, *fileExit,
        *editModel, *editHelp,
		*simulationRun,
        *toolsPrototyping, *toolsGenerateDocs,
        *viewComponents,
        *windowCloseAll, *windowCascade, *windowTile, *windowStandardize, *windowsSaveGraphics,
        *helpAbout, *helpHome;
	QSettings *settings;
		
	QLabel *permanentMessage;
	QMdiArea *_mdiArea;
    SubWindow *fileLocationsSubWindow, *viewModelSubWindow;
        QLabel *imageLabel;
        QScrollArea *scrollArea;
	
	// Data
    QString currentFilePath;
    LiveSimulation *liveSim;
    double scaleFactor;

    // Events
    void closeEvent (QCloseEvent * event);

    // Methods
    void openFile(QString filePath);
    void setTitle(QString subTitle);
	void setPermanentMessage(QString message);
    void viewModel();
    QString versionText() const;

    void tile();
    void closeSubWindows(SubWindow::Type type = SubWindow::All);
    void minimizeSubWindows(SubWindow::Type type = SubWindow::All);

    void showErrorMessage(UniSim::Exception &ex);
    void showErrorMessage(QString message);
    void showMessage(QString message);


};

inline MainWindow* mainWindow()
{
    if (!MainWindow::_mainWindow) {
   		MainWindow::_mainWindow = new MainWindow;
        UniSim::objectPool()->attach("mainWindow", MainWindow::_mainWindow);
    }
 	return MainWindow::_mainWindow;
}

#endif
