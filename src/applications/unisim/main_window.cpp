/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
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
#include "main_window.h"

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

    liveSim = new LiveSimulation(this);
    connect(liveSim, SIGNAL(stateChanged(int, int)),
            this, SLOT(liveSimulatorStateChanged(int , int)));

    useStoredGeometry();	
}

void MainWindow::createMenus() {
    // File menu
    fileMenu = menuBar()->addMenu("&File");

    fileMenu->addAction( fileOpen = new QAction("&Open...", this) );
    connect( fileOpen, SIGNAL(triggered()), this, SLOT(doFileOpen()) );

    fileMenu->addAction( fileReopen = new QAction("&Reopen..", this) );
    connect( fileReopen, SIGNAL(triggered()), this, SLOT(doFileReopen()) );

//    fileMenu->addAction( fileReopenRun = new QAction("Reopen and r&un...", this) );
//    connect( fileReopenRun, SIGNAL(triggered()), this, SLOT(doFileReopenRun()) );

    fileMenu->addAction( fileClose = new QAction("&Close", this) );
    connect( fileClose, SIGNAL(triggered()), this, SLOT(doFileClose()) );

    fileMenu->addAction( fileLocations = new QAction("&Locations...", this) );
    connect( fileLocations, SIGNAL(triggered()), this, SLOT(doFileLocations()) );

    fileMenu->addAction( fileExit = new QAction("E&xit", this) );
    connect( fileExit, SIGNAL(triggered()), this, SLOT(doFileExit()) );

    // Edit menu
    editMenu = menuBar()->addMenu("&Edit");

    editMenu->addAction( editModel = new QAction("&Model..", this) );
    connect( editModel, SIGNAL(triggered()), this, SLOT(doEditModel()) );

    editMenu->addAction( editHelp = new QAction("&Help..", this) );
    connect( editHelp, SIGNAL(triggered()), this, SLOT(doEditHelp()) );

    // Simulation menu
    simulationMenu = menuBar()->addMenu("&Simulation");

    simulationMenu->addAction( simulationRun = new QAction("&Run...", this) );
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
    // viewMenu = menuBar()->addMenu("&View");

    // Window menu
    windowMenu = menuBar()->addMenu("&Window");

    windowMenu->addAction( windowCascade = new QAction("&Cascade", this) );
    connect( windowCascade, SIGNAL(triggered()), _mdiArea, SLOT(cascadeSubWindows()) );

    windowMenu->addAction( windowTile = new QAction("&Tile", this) );
    connect( windowTile, SIGNAL(triggered()), _mdiArea, SLOT(tileSubWindows ()) );

    windowMenu->addAction( windowsSaveGraphics = new QAction("Save &graphics", this) );
    connect( windowsSaveGraphics, SIGNAL(triggered()), this, SLOT(doWindowsSaveGraphics()) );

    windowMenu->addAction( windowCloseAll = new QAction("Close &all", this) );
    connect( windowCloseAll, SIGNAL(triggered()), _mdiArea, SLOT(closeAllSubWindows()) );

    if (isDeveloperVersion()) {
        windowMenu->addAction( windowStandardize = new QAction("&Standardize", this) );
        connect( windowStandardize, SIGNAL(triggered()), this, SLOT(standardizeSubWindows()) );
    }

    // Help menu
    helpMenu = menuBar()->addMenu("&Help");

    helpMenu->addAction( helpHome = new QAction("&Home...", this) );
    connect( helpHome, SIGNAL(triggered()), this, SLOT(doHelpHome()) );

    helpMenu->addAction( helpAbout = new QAction("&About...", this) );
    connect( helpAbout, SIGNAL(triggered()), this, SLOT(doHelpAbout()) );
}

PlotWidget* MainWindow::createPlotWidget(QString title) {
    SubWindow *subWindow = new SubWindow(_mdiArea, title, SubWindow::SimulationOutput);

    QwtPlot *plot = new QwtPlot(subWindow);
    subWindow->setWidget(plot);

    return new PlotWidget(plot, subWindow);
}

void MainWindow::tile() {
//    _mdiArea->tileSubWindows();
    QList<SubWindow*> tiles = subWindowList(SubWindow::SimulationOutput);
    int rows = 2, columns = 2, n = tiles.size();
    while (rows*columns < n) {
        if (rows == columns)
            ++columns;
        else
            ++rows;
    }

    QSize scene = centralWidget()->size();
    QSize tile(scene.width()/columns, scene.height()/rows);
    int ixTile = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            QPoint pos(col*tile.width(), row*tile.height());
            if (ixTile < n) {
                tiles[ixTile]->resize(tile);
                tiles[ixTile]->move(pos);
                ++ixTile;
            }
        }

    }

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
    event->accept();
}

QList<SubWindow*> MainWindow::subWindowList(SubWindow::Type type) {
    QList<QMdiSubWindow *> candidates = _mdiArea->subWindowList();
    QList<SubWindow*> result;
    for (int i = 0; i < candidates.size(); ++i) {
        SubWindow *win = dynamic_cast<SubWindow*>(candidates[i]);
        if (win && win->type() == type)
            result << win;
    }
    return result;
}

void MainWindow::closeSubWindows(SubWindow::Type type) {
    if (type == SubWindow::All) {
        _mdiArea->closeAllSubWindows();
        return;
    }
    QList<SubWindow*> windows = subWindowList(type);
    for (int i = 0; i < windows.size(); ++i) {
        windows[i]->close();
    }
}

void MainWindow::minimizeSubWindows(SubWindow::Type type) {
    if (type == SubWindow::All) {
        _mdiArea->closeAllSubWindows();
        return;
    }
    QList<SubWindow*> windows = subWindowList(type);
    for (int i = 0; i < windows.size(); ++i) {
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

void MainWindow::doFileReopenRun() {
    // This function is not used
    // doSimulationRun() must wait for graph drawing process to end
    doFileReopen();
    doSimulationRun();
}

void MainWindow::openFile(QString filePath) {
    currentFilePath = filePath;

    if (liveSim->state() != LiveSimulation::Closed) doFileClose();

    setTitle(QFileInfo(filePath).fileName());
    FileLocationInfo::setLocation(FileLocationInfo::Models, filePath);

    try {
        liveSim->open(filePath);
        liveSim->writeGraph();
    }
    catch (Exception &ex) {
        showErrorMessage(ex);
    }

    QSettings().setValue("latestModelFile", filePath);
}


void  MainWindow::viewModel() {
    if (!viewModelSubWindow)
        viewModelSubWindow = new SubWindow(_mdiArea, "Model view", SubWindow::ModelView);

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

    viewModelSubWindow->adjustSize();
    viewModelSubWindow->showMaximized();
}

void MainWindow::doWindowsSaveGraphics() {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QList<QMdiSubWindow *> windows = _mdiArea->subWindowList();

    int fileNo = 0;
    try {
        for (int i = 0; i < windows.size(); ++i) {
            SubWindow *subWindow = dynamic_cast<SubWindow*>(windows[i]);
            if (!subWindow)
                continue;
            if (subWindow->type() == SubWindow::SimulationOutput) {
                QPixmap pixmap = QPixmap::grabWidget(windows[i]->widget());
                if (pixmap.isNull())
                    throw Exception("Could not grab graphics for saving");
                QString filePath = QString("%1/%2-graphics-%3.png").arg(path).arg(++fileNo).arg(windows[i]->windowTitle());
                bool ok = pixmap.save(filePath);
                if (!ok)
                    throw Exception("Could not save graphics file to:\n" + filePath);
            }
            else if (subWindow->type() == SubWindow::ModelView) {
                QString sourceFilePath = liveSim->graphFilePath();
                QString destFilePath = QString("%1/%2-model diagram.png").arg(path).arg(++fileNo);
                QFile::remove(destFilePath);
                bool ok = QFile::copy(sourceFilePath, destFilePath);
                if (!ok)
                    throw Exception("Could not write file with model diagram to:\n" + destFilePath);
            }
        }
    }
    catch (UniSim::Exception &ex) {
        showErrorMessage(ex);
    }

    if (fileNo == 0)
        showErrorMessage("No graphics found on screen for saving");
    else
        showErrorMessage(QString("%1 file(s) written to folder %2").arg(fileNo).arg(path));

}

void MainWindow::doFileClose()
{
    setTitle("");
    liveSim->close();
    closeSubWindows();
}

void MainWindow::doFileLocations()
{
    if (!fileLocationsSubWindow)
        fileLocationsSubWindow = new FileLocationsSubWindow(_mdiArea);
    fileLocationsSubWindow->adjustSize();
    fileLocationsSubWindow->show();
}

void MainWindow::doFileExit()
{
	close();
}

void MainWindow::doEditModel() {
    if (currentFilePath.isEmpty())
        showMessage("Use menu File|Open to open a model file");
    else
        QDesktopServices::openUrl(QUrl("file:///" + currentFilePath));
}

void MainWindow::doEditHelp() {
    QString msg =
            "If nothing happens when you select Edit|Model on the menu, "
            "or if the model file does not open in an editor, it is because the file type of model files (which is XML) "
            "is not associated with an editor program, such as Notepad or Notepad++.\n\n"
            "To fix this, choose File|Open and right-click any model file. "
            "On the pop-up menu, choose 'Open with...' and select a text editor from the list of programs. "
            "Remember to check the box 'Always use the selected program to open this kind of file'.\n\n"
            "From now on, Edit|Model should work correctly. Depending on your operating system, "
            "the steps may vary from the above.";
    showMessage(msg);

}

void MainWindow::doSimulationRun()
{
    closeSubWindows(SubWindow::ModelView);
    closeSubWindows(SubWindow::SettingsView);
    try {
        liveSim->run();
    }
    catch (Exception &ex) {
        showErrorMessage(ex);
    }
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
        showErrorMessage(ex);
    }
    if (ok)
        showMessage("Prototype successfully written to " + maker.destinationFolder());
}

void MainWindow::doToolsGenerateDocs() {
    bool ok = true;
    DocumentationWriter writer;
    try {
        writer.write();
    }
    catch (UniSim::Exception &ex) {
        showErrorMessage(ex);
        ok = false;
    }
    if (ok)
        showMessage("Documentation successfully written to output folder");
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
        "The diagrams shown in UniSim are produced by Graphviz software, included with the UniSim installation. "
        "See www.graphviz.org.\n\n"
        "UniSim was written in C++ and built with the GNU toolset, using Qt Creator and the Qt Library.\n\n"
        "To download documentation and source code, visit www.ecolmod.org.\n\n"
        "Main author followed by co-authors and contributors in alphabetical order:\n";

    const Authors::Collection *theAuthors;
    try {
        Authors::Author niels = authors()->find("Niels");
        text = addAuthor(text, niels);

        theAuthors = authors()->collection();
        QMapIterator<Identifier, Authors::Author> au(*theAuthors);
        while (au.hasNext()) {
            au.next();
            if (!au.value().id.equals("Niels"))
                text = addAuthor(text, au.value());
        }
    }
    catch (UniSim::Exception &ex) {
        showErrorMessage("Could not retrieve author information due to the following error:\n" + ex.message());
    }

    QMessageBox::about(this, "About Universal Simulator", text);
}

void MainWindow::doHelpHome() {
    QDesktopServices::openUrl(QUrl("http://www.ecolmod.org"));
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
    }
	else {
		setPermanentMessage("Busy...");
		statusBar()->showMessage(LiveSimulation::stateText(newState));	
	}
}

void MainWindow::showErrorMessage(Exception &ex) {
    showErrorMessage(ex.message());
}

void MainWindow::showErrorMessage(QString message) {
    QMessageBox::information(this, "Error", message);
}

void MainWindow::showMessage(QString message) {
    QMessageBox::information(this, "Information", message);
}

