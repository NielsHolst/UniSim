/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QApplication>
#include <QDir>
#include <QErrorMessage>
#include <QtGui>
#include <usbase/authors.h>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usbase/random.h>
#include <usengine/mega_factory.h>
#include "command_line.h"
#include "file_locations_forgiving.h"
#include "live_simulation.h"
#include "main_window.h"

using namespace UniSim;

void myMsgHandler(QtMsgType type, const char *msg)
{
    throw Exception(QString(msg));
}

void createSingletons(){
    objectPool()->attach(Authors::id(), new Authors);
    objectPool()->attach(Clock::id(), new Clock);
    objectPool()->attach(FileLocations::id(), new FileLocationsForgiving);
    objectPool()->attach(MegaFactory::id(), new MegaFactory);
    objectPool()->attach(Random::id(), new Random);
}

int runGui(int argc, char *argv[]) {
    QApplication app(argc, argv);
    mainWindow()->show();
    createSingletons();
    return app.exec();
}

void runSimulation(QString filePath) {
    createSingletons();
    LiveSimulation *sim = new LiveSimulation(0);
    sim->open(filePath);
    sim->run();
    delete sim;
}

int runCommand(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    CommandLine com(argc, argv);
    QString fileName = com.input();
    if (!fileName.isEmpty()) {
        QString filePath = FileLocations::location(FileLocationInfo::Models).absolutePath();
        if (QDir::isAbsolutePath(fileName))
            filePath = fileName;
        else
            filePath += "/" + fileName;
        runSimulation(filePath);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    bool hasArguments = argc > 1;
    qInstallMsgHandler(myMsgHandler);
	QCoreApplication::setOrganizationName("Aarhus University");
	QCoreApplication::setOrganizationDomain("www.agrsci.dk");
    QCoreApplication::setApplicationName("Universal Simulator");

    int result = -1;
    try {
        result = hasArguments ?
                    runCommand(argc,argv) :
                    runGui(argc, argv);
    }
    catch (Exception &ex) {
        if (hasArguments)
            std::cout << qPrintable("Program Error. Uncaught exception: " + ex.message());
        else
            QMessageBox::information(0, "Program Error", "Uncaught exception: " + ex.message());
    }
    catch (...) {
        if (hasArguments)
            std::cout << "Program Error. Uncaught exception.";
        else
            QMessageBox::information(0, "Program Error", "Uncaught exception.");
    }
    delete objectPool();
    return result;
}
