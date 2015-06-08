/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QApplication>
#include <QDir>
#include <QErrorMessage>
#include <QMessageBox>
#include <QtGlobal>
#include <QtGui>
#include <usbase/authors.h>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usbase/random.h>
#include <usbase/version.h>
#include <usengine/mega_factory.h>
#include "file_locations_forgiving.h"
#include "live_simulation.h"
#include "main_window.h"

using namespace UniSim;

void myMsgHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    QMessageBox::information(0, "Program Error", msg);
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

int main(int argc, char *argv[]) {
    qInstallMessageHandler(myMsgHandler);
    QCoreApplication::setApplicationVersion(version());
    QCoreApplication::setOrganizationName("Aarhus University");
    QCoreApplication::setOrganizationDomain("www.ecolmod.org");
    QCoreApplication::setApplicationName("Universal Simulator");
    try {
       runGui(argc, argv);
    }
    catch (Exception &ex) {
        QMessageBox::information(0, "Program Error", "Uncaught exception: " + ex.message());
    }
    catch (...) {
        QMessageBox::information(0, "Program Error", "Uncaught exception.");
    }
    delete objectPool();
    return 0;
}
