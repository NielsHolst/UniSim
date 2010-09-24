/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QTextStream>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/output_variable.h>
#include <usbase/parameter.h>
#include "../intercom_models/plant.h"
#include "output_file.h"

using namespace UniSim;

namespace intercom{
	
OutputFile::OutputFile(Identifier name, QObject *parent)
    : Output(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString("output_intercom.prn"), this, "description");
}

void OutputFile::initialize() {
    Output::initialize();
    plant = seekOne<Plant*>("*");
}

void OutputFile::cleanup() {
    openFile();
    write("INTERCOM Details\n\n");
    write("Canopy layers height\n");
    for (int i = 0; i < 5; ++i) {
        const double *heights = plant->pullVariablePtr<double>("heights");
        write(QString::number(i) + "\t" + heights[i] + "\n");
    }
    write("Canopy layer ELAI above\n");
    for (int i = 0; i < 5; ++i) {
        write(QString::number(i) + "\n");
    }
    file.close();
}

void OutputFile::openFile() {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString filePath = path + "/" + fileName;
    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write table:\n'" + filePath + "'");
}


void OutputFile::write(QString s) {
    file.write(qPrintable(s));
}

void OutputFile::writeTab() {
    file.write("\t");
}

void OutputFile::writeCR() {
    file.write("\n");
}



} //namespace

