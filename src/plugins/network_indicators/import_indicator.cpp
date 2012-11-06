/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "import_indicator.h"

using namespace UniSim;

namespace network_indicators {
	
ImportIndicator::ImportIndicator(Identifier name, QObject *parent)
	: Model(name, parent)
{
}

void ImportIndicator::amend() {
    Model *farmsParent = seekOne<Model*>("farms");
    farms = farmsParent->seekChildren<Model*>("*");

    // Create one variable for each farm
    farmIndicators.fill(0., farms.size());
    for (int i = 0; i < farms.size(); ++i) {
        Model *farm = farms[i];
        QString variableName = farm->id().label();
        double *variablePtr = &farmIndicators[i];
        new Variable<double>(variableName, variablePtr, this, "The import indicator value for a farm");
    }
}

void ImportIndicator::reset() {
    update();
}

void ImportIndicator::update() {
    for (int i = 0; i < farms.size(); ++i) {
        farmIndicators[i] = computeIndicator(farms[i]);
    }
}

double ImportIndicator::computeIndicator(UniSim::Model *farm) {
    return  farm->pullValue<double>("animalIntake") +
            farm->pullValue<double>("respiration") +
            farm->pullValue<double>("uptakeCrops");
}


} //namespace

