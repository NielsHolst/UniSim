/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QApplication>
#include <QtGui>
#include <usbase/object_pool.h>
#include <usbase/random_uniform.h>
#include <usengine/controller_maker.h>
#include <usengine/model_maker.h>
#include <usengine/output_maker.h>
#include "file_locations_forgiving.h"
#include "main_window.h"

using namespace UniSim;

void myMsgHandler(QtMsgType type, const char *msg)
{
	QMessageBox::warning(0, "Fatal Error", msg);
}

void createSingletons(){
    objectPool()->attach(FileLocations::id(), new FileLocationsForgiving);
    objectPool()->attach(ControllerMaker::id(), new ControllerMaker);
    objectPool()->attach(ModelMaker::id(), new ModelMaker);
    objectPool()->attach(OutputMaker::id(), new OutputMaker);
    objectPool()->attach(RandomUniform::id(), new RandomUniform);
}

int main(int arbc, char *argv[])
{
	qInstallMsgHandler(myMsgHandler);
	
	QApplication app(arbc, argv);

	QCoreApplication::setOrganizationName("Aarhus University");
	QCoreApplication::setOrganizationDomain("www.agrsci.dk");
    QCoreApplication::setApplicationName("Universal Simulator");

    int result = 0;
    try {
        createSingletons();
        mainWindow()->show();
        result = app.exec();
    }
    catch (UniSim::Exception &ex) {
        QMessageBox::information(0, "Program Error", "Uncaught exception: " + ex.message());
    }

    delete objectPool();

    return result;
}
