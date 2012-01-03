/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "grass_productivity_remote_estimate.h"

using namespace UniSim;


namespace AnimalStocking {

GrassProductivityRemoteEstimate::GrassProductivityRemoteEstimate(UniSim::Identifier name, QObject *parent)
    : GrassProductivity(name, parent)
{
    new Parameter<double>("slope", &slope, 3.6, this, "desc");
    new Parameter<double>("intercept", &intercept, 4.3, this, "desc");
}

double GrassProductivityRemoteEstimate::calculateDryMatter() {
    double par = environment->pullVariable<double>("ParAbs");
    return slope*par + intercept;
}

} //namespace

