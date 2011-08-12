/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QList>
#include <QMessageBox>
#include <QString>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "sensitivity_analysis.h"

namespace UniSim {
	
SensitivityAnalysis::SensitivityAnalysis(Identifier name, QObject *parent)
    : TimeStepLimited(name, parent)
{
    new Parameter<double>("factor", &factor, 2, this, "description");
}

//typedef QList<int> Subset;
//typedef QList<Subset> Subsets;
//Subsets subsets;


//bool splitSubset(const Subset &subset) {
//    for (int i = 0; i < subset.size(); ++i) {
//        QStringList items =
//    }
//}

//void splitSubsets() {
//    for (int i = 0; i < subsets.size(); ++i) {
//        if (splitSubset(subsets[i]))
//            subsets.removeAt(i);
//    }
//}

//void simplify(QStringList names) {
//    Subset all;
//    for (int i = 0; i < names.size(); ++i) {
//        all.append(i);
//    }
//    subsets.append(all);

//    do {
//        splitSubsets();
//    }
//    while (simplifySubsets());
//}


void SensitivityAnalysis::initialize() {
    TimeStepLimited::initialize();
    models = seekMany<Model*>("*");

    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString fn = path + QString("/sa-%1.txt").arg(models.size());
    //QMessageBox::information(0, "Test", fn);
    QFile file(fn);
    file.open(QIODevice::Text | QIODevice::WriteOnly);
    QStringList names;
    for (int i = 0; i < models.size(); ++i) {
        QString s = models[i]->fullName() + "\n";
        file.write(s.toAscii());
        names.append(models[i]->fullName());
    }

//    simplify(names);
}

} //namespace

