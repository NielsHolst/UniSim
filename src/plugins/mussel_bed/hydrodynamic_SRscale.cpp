/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "hydrodynamic_SRscale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

HydrodynamicSRScale::HydrodynamicSRScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("maxflow", &maxflow, 0., this, "desc");
    new Variable<double>("value", &value, this, "desc");
}

void HydrodynamicSRScale::reset() {
    value = 0.;
}

void HydrodynamicSRScale::update() {

    value = 0.;
    for (int i=0; i<=180; i++){
        value += exp(3.34522-0.08084*(maxflow*cos(DEG_TO_RAD*i))-0.81692)/(1+exp(3.34522-0.08084*(maxflow*cos(DEG_TO_RAD*i))-0.81692));
    }
    value = value/180;
}

} //namespace
