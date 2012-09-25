/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "stage.h"
#include "stage_demand.h"

namespace UniSim{
	
StageDemand::StageDemand(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QString>("stage", &stageName, QString(), this,
        "Name of stage from which the current demand is requested by the @F growthDemand method");
    new Variable<double>("value", &value, this,
        "Current demand of @F {stage}");
}

void StageDemand::initialize() {
    stage = peekOneNearest<Stage*>(stageName);
    if (!stage)
        stage = seekOne<Stage*>(stageName);
}

void StageDemand::reset() {
    value = 0.;
}

void StageDemand::update() {
    value = stage->growthDemand();
}

} //namespace

