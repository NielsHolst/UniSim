/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "indicator_base.h"

using namespace UniSim;

namespace network_indicators {
	
IndicatorBase::IndicatorBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("cv", &cv, 0., this, "Coefficient of variation");

    // Create objects for random number generation
    const double mean = 0, sd = 1;
    distribution = new Distribution(mean, sd);
    variate = new Variate(*randomGenerator(), *distribution);
}

IndicatorBase::~IndicatorBase() {
    // Delete objects for random number generation
    delete distribution;
    delete variate;
}

double IndicatorBase::rnd(double mean) {
    return mean*(1. + cv*(*variate)());
}

void IndicatorBase::amend() {
    Model *farmsParent = seekOne<Model*>("farms");
    farms = farmsParent->seekChildren<Model*>("*");

    farmIndicators.fill(0., farms.size());
    for (int i = 0; i < farms.size(); ++i) {
        Model *farm = farms[i];
        QString variableName = farm->id().label();
        double *variablePtr = &farmIndicators[i];
        new Variable<double>(variableName, variablePtr, this, "The import indicator value for a farm");
    }
}

void IndicatorBase::reset() {
    update();
}

void IndicatorBase::update() {
    for (int i = 0; i < farms.size(); ++i) {
        farmIndicators[i] = computeIndicator(farms[i]);
    }
}

} //namespace

