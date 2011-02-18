/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <usbase/authors.h>
#include <usbase/model.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/version.h>
#include <usengine/documentation_writer.h>
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

    setTitle("");
    setCentralWidget(_mdiArea = new QMdiArea(this));
    createMenus();

    statusBar()->addPermanentWidget(permanentMessage = new QLabel(this));
    setPermanentMessage("No model");

    liveSim = new LiveSimulation(this,_logSubWindow = new LogSubWindow(_mdiArea));
    connect(liveSim, SIGNAL(stateChanged(int, int)),
            this, SLOT(liveSimulatorStateChanged(int , int)));

    _logSubWindow->hide();

    useStoredGeometry();	
}

void MainWindow::createMenus() {
    // File menu
    fileMenu = menuBar()->addMenu("&File");

    fileMenu->addAction( fileOpen = new QAction("&Open", this) );
    connect( fileOpen, SIGNAL(triggered()), this, SLOT(doFileOpen()) );

    fileMenu->addAction( fileReopen = new QAction("&Reopen", this) );
    connect( fileReopen, SIGNAL(triggered()), this, SLOT(doFileReopen()) );

    fileMenu->addAction( fileClose = new QAction("&Close", this) );
    connect( fileClose, SIGNAL(triggered()), this, SLOT(doFileClose()) );

    fileMenu->addAction( fileLocations = new QAction("&Locations...", this) );
    connect( fileLocations, SIGNAL(triggered()), this, SLOT(doFileLocations()) );

    fileMenu->addAction( fileExit = new QAction("E&xit", this) );
    connect( fileExit, SIGNAL(triggered()), this, SLOT(doFileExit()) );

    // Simulation menu
    simulationMenu = menuBar()->addMenu("&Simulation");

    simulationMenu->addAction( simulationRun = new QAction("&Run", this) );
    connect( simulationRun, SIGNAL(triggered()), this, SLOT(doSimulationRun()) );

    // Tools menu
    if (isDeveloperVersion()) {
        toolsMenu = menuBar()->addMenu("&Tools");
        toolsMenu->addAction( toolsPrototyping = new QAction("&Create plugin...", this) );
        connect( toolsPrototyping, SIGNAL(triggered()), this, SLOT(doToolsPrototyping()) );

        toolsMenu->addAction( toolsGenerateDocs = new QAction("&Generate documentation...", this) );
        connect( toolsGenerateDocs, SIGNAL(triggered()), this, SLOT(doToolsGenerateDocs()) );
    }

    //View menu
    viewMenu = menuBar()->addMenu("&View");

    viewMenu->addAction( viewLog = new QAction("&Log", this) );
    connect( viewLog, SIGNAL(triggered()), this, SLOT(doViewLog()) );

    // Window menu
    windowMenu = menuBar()->addMenu("&Window");

    windowMenu->addAction( windowCascade = new QAction("&Cascade", this) );
    connect( windowCascade, SIGNAL(triggered()), _mdiArea, SLOT(cascadeSubWindows()) );

    windowMenu->addAction( windowTile = new QAction("&Tile", this) );
    connect( windowTile, SIGNAL(triggered()), _mdiArea, SLOT(tileSubWindows ()) );

    windowMenu->addAction( windowCloseAll = new QAction("Close &all", this) );
    connect( windowCloseAll, SIGNAL(triggered()), _mdiArea, SLOT(closeAllSubWindows()) );

    if (isDeveloperVersion()) {
        windowMenu->addAction( windowStandardize = new QAction("&Standardize", this) );
        connect( windowStandardize, SIGNAL(triggered()), this, SLOT(standardizeSubWindows()) );
    }

    // Help menu
    helpMenu = menuBar()->addMenu("&Help");

    helpMenu->addAction( helpAbout = new QAction("&About", this) );
    connect( helpAbout, SIGNAL(triggered()), this, SLOT(doHelpAbout()) );
}

PlotWidget* MainWindow::createPlotWidget(QString title) {
    SubWindow *subWindow = new SubWindow(_mdiArea, title);
    subWindow->setType(SubWindow::Output);

    QwtPlot *plot = new QwtPlot(_mdiArea);
    subWindow->setWidget(plot);

    return new PlotWidget(plot, subWindow);
}

void MainWindow::tile() {
    _mdiArea->tileSubWindows();
}

void MainWindow::setTitle(QString subTitle) {
    QString title;
    if (!subTitle.isEmpty()) title = subTitle + "  -  ";

    title += "Universal Simulator ";
    title += versionExtended();
    setWindowTitle(title);
}

void MainWindow::closeEvent (QCloseEvent * event) {
    store();
    for (int i = 0; i <_xmlEditors.size(); ++i)
        delete _xmlEditors[i];
    _xmlEditors.clear();
    event->accept();
}

void MainWindow::closeSubWindows(SubWindow::Type type) {
    if (type == SubWindow::All) {
        _mdiArea->closeAllSubWindows();
        return;
    }
    QList<QMdiSubWindow *> windows = _mdiArea->subWindowList();
    for (int i = 0; i < windows.size(); ++i) {
        SubWindow *subWindow = dynamic_cast<SubWindow*>(windows[i]);
        bool doClose = subWindow && subWindow->type() == type;
        if (doClose)
            subWindow->close();
    }
}

void MainWindow::minimizeSubWindows(SubWindow::Type type) {
    QList<QMdiSubWindow *> windows = _mdiArea->subWindowList();
    for (int i = 0; i < windows.size(); ++i) {
        SubWindow *subWindow = dynamic_cast<SubWindow*>(windows[i]);
        bool doMinimize = type == SubWindow::All ||
                          ( subWindow && subWindow->type() == type );
        if (doMinimize)
            windows[i]->showMinimized();
    }
}

void MainWindow::standardizeSubWindows() {
    const int height = 250;
    const int width = 480;
    QList<QMdiSubWindow *> windows = _mdiArea->subWindowList();
    for (int i = 0; i < windows.size(); ++i)
        windows[i]->resize(width, height);
}

void MainWindow::setPermanentMessage(QString message) {
	permanentMessage->setText(message);
}

void MainWindow::doFileOpen() {
    QString folder = FileLocations::possibleLocation(FileLocationInfo::Models).absolutePath();
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
    FileLocationInfo::setLocation(FileLocationInfo::Models, filePath);

    liveSim->open(filePath);
    liveSim->writeGraph();

    QSettings().setValue("latestModelFile", filePath);
}



void  MainWindow::viewModel() {
    if (!viewModelSubWindow) viewModelSubWindow = new SubWindow(_mdiArea, "Model view");

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    viewModelSubWindow->setWidget(scrollArea);

    QImage image(liveSim->graphFilePath());
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;
    scrollArea->setWidgetResizable(true);

    viewModelSubWindow->setType(SubWindow::View);
    viewModelSubWindow->adjustSize();
    viewModelSubWindow->showMaximized();
}

void MainWindow::doFileEdit() {
    if (liveSim->state() != LiveSimulation::Ready)
        doFileOpen();
    if (liveSim->state() != LiveSimulation::Ready)
        return;
    editFile(FileLocations::location(FileLocationInfo::Models).absolutePath());
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
    tile();
}

void MainWindow::doToolsPrototyping() {
    QString folder = FileLocations::possibleLocation(FileLocationInfo::Prototypes).absolutePath();
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open prototype file",
                                                    folder,
                                                    "Prototype files (*.xml)");
    if (filePath.isEmpty()) return;
    FileLocationInfo::setLocation(FileLocationInfo::Prototypes, filePath);

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
    bool ok = true;
    DocumentationWriter writer;
    try {
        writer.write();
    }
    catch (UniSim::Exception &ex) {
        ok = false;
        LogBase::LogItem message = { "Error", ex.message() };
        _logSubWindow->tell(message);
    }
    if (ok) QMessageBox::information(
            this,
            "Message",
            "Documentation successfully written to output folder");
}

void MainWindow::doViewLog()
{
	_logSubWindow->adjustSize();
	_logSubWindow->show();
}

namespace {
    QString addAuthor(QString s, const Authors::Author &author) {
        return s + "- " +
                author.name + ", " +
                author.address +"\n";
    }
}

void MainWindow::doHelpAbout() {
    QString text =
        "Universal Simulator (UniSim) " + versionExtended() + "\n\n"
        "Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors. Copyrights reserved.\n\n"
        "Released under the terms of the GNU General Public License version 3.0 or later. "
        "See www.gnu.org/copyleft/gpl.html.\n\n"
        "The plots drawn by UniSim are based on the Qwt library. See qwt.sourceforge.net.\n\n"
        "The diagrams shown in UniSim are produced by Graphviz software, included with the UniSim installation. See www.graphviz.org.\n\n";


    text += "Main author followed by co-authors in alphabetical order:\n";
    Authors::Author mainAuthor =   {"Niels",
                        "Niels Holst",
                        "Aarhus University, Flakkebjerg, Denmark",
                        "niels.holst@agrsci.dk"};
    text = addAuthor(text, mainAuthor);

    const Authors::Collection *theAuthors;
    try {
        theAuthors = authors()->collection();
        QMapIterator<Identifier, Authors::Author> au(*theAuthors);
        while (au.hasNext()) {
            au.next();
            text = addAuthor(text, au.value());
        }
    }
    catch (UniSim::Exception &ex) {
        QMessageBox::information(this, "Message", "Could not retrieve author information due to the following error:\n" + ex.message());
    }

    QMessageBox::about(this, "About Universal Simulator", text);
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
