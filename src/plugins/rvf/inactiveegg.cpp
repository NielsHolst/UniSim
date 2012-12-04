/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "inactiveegg.h"

using namespace UniSim;

namespace rvf {

InactiveEgg::InactiveEgg(Identifier name, QObject *parent)
  : Model(name, parent)
{
    new Parameter<double>("initialDensity", &initialDensity, 0., this, "desc");
    new Parameter<double>("lossRate", &lossRate, 20., this, "desc"); //lossRate is proportion lost per day
    new Parameter<double>("activationRate", &activationRate, 20., this, "desc");//activationRate is proportion activited per day
    new Parameter<double>("inflow", &inflow, 0., this, "desc");
    new Variable<double>("density", &density, this, "desc");
    new Variable<double>("eggsLost", &eggsLost, this, "desc");
    new Variable<double>("eggsActivated", &eggsActivated, this, "desc");
}

void InactiveEgg::reset() {
    density = initialDensity;
    eggsLost = 0.;
    eggsActivated = 0.;
}

void InactiveEgg::update() {
    density += inflow;
    eggsLost = lossRate*density;
    density -= eggsLost;
    eggsActivated = activationRate*density;
    density -= eggsActivated;
}

} //namespace
