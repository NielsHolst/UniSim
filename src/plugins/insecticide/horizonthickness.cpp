/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usengine/simulation.h>
#include "horizonthickness.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (horizonthickness)
	
horizonthickness::horizonthickness(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input (double, penetrationDepth, 0.5);
    Input (QString, soilFileName, "Htc.txt");
    Input (QString, soilType, "PestLCI1Tune");
    for (int i=0; i <10; ++i) {
        QString name = "Htc" + QString::number(i);
        Output2 (double, Htc[i], name);    // In units of "m"
    }
}

void horizonthickness::initialize() {
    // Fetch soil parameters for soil type
    QString absFileName = simulation()->inputFilePath(soilFileName);
    soilTable = new DataGrid(absFileName, this);
    QString soilTypeL = soilType.toLower();
    QString rowSoilTypeL;
    int row;
    for (row=0; row<soilTable->rowNumber(); ++row) {
        rowSoilTypeL = soilTable->cell<QString>(row,0).toLower();
        if (soilTypeL==rowSoilTypeL) break;
    }
    if (soilTypeL!=rowSoilTypeL) {
        QString msg("Soil type '%1' not found in file '%2'");
        throw Exception(msg.arg(soilType).arg(absFileName));
    }
    // Extract parameter values from this row
    bulkDensity = soilTable->cell<double>(row,62);
    for (int i=0; i<11; ++i)
        start[i] =  soilTable->cell<double>(row,i+1);
    for (int i=0; i<10; ++i) {
        pH[i] =  soilTable->cell<double>(row,i+12);
        foc[i] =  soilTable->cell<double>(row,i+52);
        int k = 3*i+22;
        clay[i] =  soilTable->cell<double>(row,k);
        silt[i] =  soilTable->cell<double>(row,k+1);
        sand[i] =  soilTable->cell<double>(row,k+2);
    }
}

void horizonthickness::reset() {
    for (int i=0; i <10; ++i)
        Htc[i] = start[i+1] - start[i] - penetrationDepth;
}

void horizonthickness::update() {
    // It seems like the time step should also enter this
    for (int i=0; i <9; ++i)
        Htc[i] = Htc[i+1] - Htc[i];
}

} //namespace

