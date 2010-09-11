/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    class PlotWidget;
    class XmlEditor;
}

class MainWindow : public QMainWindow,
                   public UniSim::MainWindowInterface,
                   public StoredWidget
{
	Q_OBJECT

public:
    void createMenus();
    UniSim::PlotWidget* createPlotWidget(QString title);
    void closeSubWindows(SubWindow::Type type = SubWindow::All);
    void minimizeSubWindows(SubWindow::Type type = SubWindow::All);

private slots:
	void doFileOpen();
    void doFileReopen();
    void doFileEdit();
    void doFileLocations();
    void doFileClose();
	void doFileExit();
	void doSimulationRun();
    void doToolsPrototyping();
    void doToolsGenerateDocs();
	void doViewLog();
    void doHelpAbout();
    void standardizeSubWindows();

    void liveSimulatorStateChanged(int oldState, int newState);

	
private:
	// Singleton
	MainWindow();
	static MainWindow* _mainWindow;
	friend MainWindow* mainWindow();
	
	// Main window GUI
    QMenu *fileMenu, *simulationMenu, *toolsMenu, *viewMenu, *windowMenu, *helpMenu;
	QAction 
        *fileOpen, *fileReopen, *fileEdit, *fileClose, *fileLocations, *fileExit,
		*simulationRun,
        *toolsPrototyping, *toolsGenerateDocs,
		*viewComponents, *viewLog,
        *windowCloseAll, *windowCascade, *windowTile, *windowStandardize,
        *helpAbout;
	QSettings *settings;
		
	QLabel *permanentMessage;
	QMdiArea *_mdiArea;
	LogSubWindow *_logSubWindow;
    SubWindow *fileLocationsSubWindow, *viewModelSubWindow;
        QLabel *imageLabel;
        QScrollArea *scrollArea;
	
	// Data
    QList<UniSim::XmlEditor*> _xmlEditors;
	LiveSimulation *liveSim;
    double scaleFactor;

    // Events
    void closeEvent (QCloseEvent * event);

    // Methods
    void openFile(QString filePath);
    void editFile(QString filePath);
    void setTitle(QString subTitle);
	void setPermanentMessage(QString message);
    void viewModel();
    QString versionText() const;
	
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
