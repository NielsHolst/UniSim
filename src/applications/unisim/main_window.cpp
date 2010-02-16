/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <QFileInfo>
#include <QtGui>
#include <QSettings>
#include <QVector>
#include <qwt_plot.h>
#include <usbase/model.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usengine/model_documentation_writer.h>
#include <usengine/prototype_maker.h>
#include <usengine/plot_widget.h>  // test
#include "file_locations_sub_window.h"
#include "image_widget.h"
#include "live_simulation.h"
#include "log_sub_window.h"
#include "main_window.h"
#include "xml_editor.h"

using namespace UniSim;

MainWindow* MainWindow::_mainWindow;

MainWindow::MainWindow()
    : StoredWidget(this, "geometries/main"),
    fileLocationsSubWindow(0),
    viewModelSubWindow(0)
{
	settings = new QSettings(this);

	setCentralWidget(_mdiArea = new QMdiArea(this));
    liveSim = new LiveSimulation(this,_logSubWindow = new LogSubWindow(_mdiArea));
    _logSubWindow->hide();
    connect(liveSim, SIGNAL(stateChanged(int, int)),
            this, SLOT(liveSimulatorStateChanged(int , int)));

    setTitle("");
	
    fileMenu 		= menuBar()->addMenu("&File");
    simulationMenu  = menuBar()->addMenu("&Simulation");
    toolsMenu		= menuBar()->addMenu("&Tools");
    viewMenu		= menuBar()->addMenu("&View");
	windowMenu		= menuBar()->addMenu("&Window");
    //helpMenu		= menuBar()->addMenu("&Help");
	
	fileMenu->addAction( fileOpen = new QAction("&Open", this) );
	connect( fileOpen, SIGNAL(triggered()), this, SLOT(doFileOpen()) );
	
    fileMenu->addAction( fileReopen = new QAction("&Reopen", this) );
    connect( fileReopen, SIGNAL(triggered()), this, SLOT(doFileReopen()) );

    /*fileMenu->addAction( fileEdit = new QAction("&Edit", this) );
    connect( fileEdit, SIGNAL(triggered()), this, SLOT(doFileEdit()) );*/

    fileMenu->addAction( fileClose = new QAction("&Close", this) );
	connect( fileClose, SIGNAL(triggered()), this, SLOT(doFileClose()) );

    fileMenu->addAction( fileLocations = new QAction("&Locations...", this) );
    connect( fileLocations, SIGNAL(triggered()), this, SLOT(doFileLocations()) );

    fileMenu->addAction( fileExit = new QAction("E&xit", this) );
	connect( fileExit, SIGNAL(triggered()), this, SLOT(doFileExit()) );

	simulationMenu->addAction( simulationRun = new QAction("&Run", this) );
	connect( simulationRun, SIGNAL(triggered()), this, SLOT(doSimulationRun()) );

    toolsMenu->addAction( toolsPrototyping = new QAction("&Create plugin...", this) );
    connect( toolsPrototyping, SIGNAL(triggered()), this, SLOT(doToolsPrototyping()) );

    toolsMenu->addAction( toolsGenerateDocs = new QAction("&Generate documentation...", this) );
    connect( toolsGenerateDocs, SIGNAL(triggered()), this, SLOT(doToolsGenerateDocs()) );

    /*viewMenu->addAction( viewComponents = new QAction("&Components", this) );
    connect( viewComponents, SIGNAL(triggered()), this, SLOT(doViewComponents()) );*/

	viewMenu->addAction( viewLog = new QAction("&Log", this) );
	connect( viewLog, SIGNAL(triggered()), this, SLOT(doViewLog()) );

	windowMenu->addAction( windowCascade = new QAction("&Cascade", this) );
	connect( windowCascade, SIGNAL(triggered()), _mdiArea, SLOT(cascadeSubWindows ()) );
	
	windowMenu->addAction( windowTile = new QAction("&Tile", this) );
	connect( windowTile, SIGNAL(triggered()), _mdiArea, SLOT(tileSubWindows ()) );

	windowMenu->addAction( windowCloseAll = new QAction("Close &all", this) );
	connect( windowCloseAll, SIGNAL(triggered()), _mdiArea, SLOT(closeAllSubWindows ()) );

	useStoredGeometry();		
	
	statusBar()->addPermanentWidget(permanentMessage = new QLabel(this));
	setPermanentMessage("No model");	
}

PlotWidget* MainWindow::createPlotWidget(QString title) {
    SubWindow *subWindow = new SubWindow(_mdiArea, title);
    subWindow->setType(SubWindow::Output);

    QwtPlot *plot = new QwtPlot(_mdiArea);
    subWindow->setWidget(plot);

    return new PlotWidget(plot, subWindow);
}

void MainWindow::setTitle(QString subTitle) {
    QString title;
    if (!subTitle.isEmpty()) title = subTitle + "  -  ";

    title += "Universal Simulator";
    setWindowTitle(title);
}

void MainWindow::closeEvent (QCloseEvent * event) {
    for (int i = 0; i <_xmlEditors.size(); ++i)
        delete _xmlEditors[i];
    _xmlEditors.clear();
    event->accept();
}

void MainWindow::closeSubWindows() {
    _mdiArea->closeAllSubWindows();
}

void MainWindow::closeSubWindows(SubWindow::Type type) {
    QList<QMdiSubWindow *> subList = _mdiArea->subWindowList();
    for (int i = 0; i < subList.size(); ++i) {
        SubWindow *subWindow = dynamic_cast<SubWindow*>(subList[i]);
        if (subWindow && subWindow->type() == type) subWindow->close();
    }
}

void MainWindow::minimizeSubWindows() {
    QList<QMdiSubWindow *> subList = _mdiArea->subWindowList();
    for (int i = 0; i < subList.size(); ++i) {
        SubWindow *subWindow = dynamic_cast<SubWindow*>(subList[i]);
        if (subWindow) subWindow->showMinimized();
    }
}

void MainWindow::minimizeSubWindows(SubWindow::Type type) {
    QList<QMdiSubWindow *> subList = _mdiArea->subWindowList();
    for (int i = 0; i < subList.size(); ++i) {
        SubWindow *subWindow = dynamic_cast<SubWindow*>(subList[i]);
        if (subWindow && subWindow->type() == type) subWindow->showMinimized();
    }
}

void MainWindow::setPermanentMessage(QString message) {
	permanentMessage->setText(message);
}
	
MainWindow::~MainWindow()
{
}

void MainWindow::doFileOpen() {
    QString folder = FileLocations::possibleLocation(FileLocations::Models).absolutePath();
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open model file",
                                                    folder,
                                                    "Model files (*.xml)");
    if (filePath.isEmpty()) return;
    openFile(filePath);
}

void MainWindow::doFileReopen() {
    QSettings settings;
    if (settings.contains("latestModelFile"))
        openFile(settings.value("latestModelFile").toString());
    else
        doFileOpen();
}

void MainWindow::openFile(QString filePath)
{
    if (liveSim->state() != LiveSimulation::Closed) doFileClose();

    setTitle(QFileInfo(filePath).fileName());
    FileLocations::setLocation(FileLocations::Models, filePath);
    liveSim->open(filePath);
    liveSim->writeGraph();
    QSettings().setValue("latestModelFile", filePath);
}



void  MainWindow::viewModel() {
    if (!viewModelSubWindow) viewModelSubWindow = new SubWindow(_mdiArea, "Model view");
    UniSim::ImageWidget *image = new UniSim::ImageWidget;
    try {
        image->setImage(liveSim->graphFilePath());
    }
    catch (UniSim::Exception &ex) {
        LogBase::LogItem message = { "Warning", ex.message() };
        _logSubWindow->tell(message);
        return;
    }

    viewModelSubWindow->setType(SubWindow::View);
    viewModelSubWindow->setWidget(image);
    viewModelSubWindow->adjustSize();
    viewModelSubWindow->showNormal();
}

void MainWindow::doFileEdit() {
    if (liveSim->state() != LiveSimulation::Ready)
        doFileOpen();
    if (liveSim->state() != LiveSimulation::Ready)
        return;
    editFile(FileLocations::location(FileLocations::Models).absolutePath());
}

void MainWindow::editFile(QString filePath)
{
    _xmlEditors.append(new XmlEditor);
    _xmlEditors.last()->execute(filePath);
}

void MainWindow::doFileClose()
{
    setTitle("");
    liveSim->close();
    _mdiArea->closeAllSubWindows();
}

void MainWindow::doFileLocations()
{
    if (!fileLocationsSubWindow)
        fileLocationsSubWindow = new FileLocationsSubWindow(_mdiArea);
    fileLocationsSubWindow->adjustSize();
    fileLocationsSubWindow->showNormal();
}

void MainWindow::doFileExit()
{
	close();
}

void MainWindow::doSimulationRun()
{
    minimizeSubWindows(SubWindow::View);
    liveSim->run();
    _mdiArea->tileSubWindows();
}

void MainWindow::doToolsPrototyping() {
    QString folder = FileLocations::possibleLocation(FileLocations::Prototypes).absolutePath();
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open prototype file",
                                                    folder,
                                                    "Prototype files (*.xml)");
    if (filePath.isEmpty()) return;
    FileLocations::setLocation(FileLocations::Prototypes, filePath);

    bool ok = true;
    UniSim::PrototypeMaker maker;
    try {
        maker.parse(filePath);
    }
    catch (UniSim::Exception &ex) {
        ok = false;
        LogBase::LogItem message = { "Error", ex.message() };
        _logSubWindow->tell(message);
    }
    if (ok) {
        QString msg = "Prototype successfully written to " +
                      maker.destinationFolder();
        QMessageBox::information(this, "Message", msg);
    }
}

void MainWindow::doToolsGenerateDocs() {
    QDir dir = FileLocations::location(FileLocations::Output);
    QString folderPath = dir.absolutePath();
    QString filePath = folderPath + "/" + "index.html";

    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!file.isOpen()) {
        QString msg = "Cannot open file to write documentation: " +
                      filePath;
        QMessageBox::information(this, "Message", msg);
        return;
    }

    ModelDocumentationWriter writer;
    writer.setDevice(&file);

    writer.write();

    QString msg = "Documentation successfully written to " +
                  filePath;
    QMessageBox::information(this, "Message", msg);

}

void MainWindow::doViewComponents()
{
    /*
	QListWidget *list = new QListWidget(this);
	list->addItems(UniSim::ModelMaker::selection());

    QStringList types = UniSim::ModelMaker::selection();
    QObject all;
    for (int i = 0; i < types.size(); ++i) {
        UniSim::Model *model = UniSim::ModelMaker::create(types[i], "view", &all);
        model->initialize();
        QStringList parameters = model->allParameters();
        list->addItem(new QListWidgetItem(types[i], list));
        for (int j = 0; j < parameters.size(); ++j)
            list->addItem(new QListWidgetItem("P: "+parameters[j], list));
    }

	SubWindow *subWin = new SubWindow(_mdiArea, "Available model components");
	subWin->setWidget(list);
	subWin->adjustSize();
	subWin->show();
    */
	//_mdiArea->addSubWindow(list)->show();
}

void MainWindow::doViewLog()
{
	_logSubWindow->adjustSize();
	_logSubWindow->show();
}

void MainWindow::liveSimulatorStateChanged(int iOldState, int iNewState) {
    LiveSimulation::State newState = (LiveSimulation::State) iNewState;

    if (newState == LiveSimulation::EndViewBuilding)
        viewModel();

	if (newState == LiveSimulation::Ready)
		setPermanentMessage("Ready");
	else if (newState == LiveSimulation::Closed)
		setPermanentMessage("No model");
    else if (newState == LiveSimulation::Error) {
		setPermanentMessage("Error");
        doViewLog();
    }
	else {
		setPermanentMessage("Busy...");
		statusBar()->showMessage(LiveSimulation::stateText(newState));	
	}
}


