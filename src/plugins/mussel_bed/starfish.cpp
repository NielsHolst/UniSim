/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "starfish.h"

using namespace UniSim;

namespace mussel_bed {

Starfish::Starfish(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("inDensity", &inDensity, 0.3, this, "desc"); //initial density of starfish (kg/m2)
    new Parameter<double>("mortality", &mortality, 0., this, "desc"); //natural mortality of starfish rate (per day)
    new Parameter<double>("stgrowthRate", &stgrowthRate, 0.03, this, "desc"); //starfish population growht rate (per day)
    new Variable<double>("stdensity", &stdensity, this, "desc"); //starfish density
}
void Starfish::reset() {
    stdensity = inDensity;
}

void Starfish::update() {
    stdensity += stgrowthRate - mortality;
}


} //namespace

