/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH SEARCH RATE SCALING MODEL: This model attend to the physical limitation that salinity produce in
   starfish capacity to prey and food searching. From Agüera et al. (under review)*/

#include "salinity_SRscale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

SalinitySRScale::SalinitySRScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("Smax", &Smax, 31., this, "desc");
    new Parameter<double>("Smin", &Smin, 10., this, "desc");
    new Variable<double>("value", &value, this, "desc");
}

void SalinitySRScale::reset() {
    value = 1;
}

void SalinitySRScale::update() {
    double Sal = (Smax+Smin)*0.5;
    if (Sal>= 10){
        value = exp(-10.92728+0.6211*Sal)/(1+exp(-10.92728+0.6211*Sal));
    }
    else
    value = 0.;
    }

} //namespace
