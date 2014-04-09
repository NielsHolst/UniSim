/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_location_info.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "../unisim/stage_base.h"
#include "life_stage.h"

using namespace UniSim;


namespace beehive {

LifeStage::LifeStage(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<QString>(Name(outputFileName), QString(),
        "Name of output file with daily size distribution. Ignored if empty.");
    addVariable<double>(Name(size),
        "Average size of an individual (mg)");
}

void LifeStage::initialize() {
    number = seekOneChild<Model*>("number");
    mass = seekOneChild<Model*>("mass");
    growth = peekOneChild<Model*>("growth");
}

void LifeStage::reset() {
    size = 0.;
    if (hasOutput()) {
        QDir dir = FileLocationInfo::getLocation(FileLocationInfo::Output);
        QString filePath = dir.absoluteFilePath(outputFileName);
        f.setFileName(filePath);
        f.open(QIODevice::WriteOnly | QIODevice::Text);
    }
}

void LifeStage::update() {
    double m = mass->pullValue<double>("value");
    double n = number->pullValue<double>("value");
    size = (n > 1e-12) ? m/n : 0.;
    if (growth) {
        double g = growth->pullValue<double>("value");
        if (g > 0)
            mass->pushValue<double>("growthRate", g);
    }
    if (hasOutput())
        writeSizeDistribution();
}

void LifeStage::cleanup() {
    if (hasOutput())
        f.close();
}

bool LifeStage::hasOutput() {
    return !outputFileName.isEmpty();
}

void LifeStage::writeSizeDistribution() {
    const double *m = dynamic_cast<StageBase*>(mass)->data();
    const double *n = dynamic_cast<StageBase*>(number)->data();
    int k = mass->pullValue<int>("k");

    int t = seekOne<Model*>("calendar")->pullValue<int>("totalTimeSteps");
    f.write(qPrintable(QString::number(t) + "\t" + id().label() + "\t"));
    for (int i = 0; i < k; ++i) {
        double size = (n[i] > 1e-12) ? m[i]/n[i] : 0.;
        QString delim = (i < k-1) ? "\t" : "\n";
        f.write(qPrintable(QString::number(size) + delim));
    }
}


} //namespace

