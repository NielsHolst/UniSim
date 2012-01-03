/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "grass_productivity_cutting_estimate.h"

using namespace UniSim;


namespace AnimalStocking {

GrassProductivityCuttingEstimate::GrassProductivityCuttingEstimate(UniSim::Identifier name, QObject *parent)
    : GrassProductivity(name, parent)
{
    new Parameter<double>("square", &square, -0.7, this, "desc");
    new Parameter<double>("slope", &slope, 9.07, this, "desc");
    new Parameter<double>("intercept", &intercept, -11.7, this, "desc");
}

double GrassProductivityCuttingEstimate::calculateDryMatter() {
    double h = environment->pullVariable<double>("GrassHeight");
    return square*h*h + slope*h + intercept;
}

} //namespace

