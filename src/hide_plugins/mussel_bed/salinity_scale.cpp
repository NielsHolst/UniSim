/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "salinity_scale.h"

using namespace UniSim;

namespace mussel_bed {

SalinityScale::SalinityScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("Smax", &Smax, 30., this, "desc");
    new Parameter<double>("Smin", &Smin, 24., this, "desc");
    new Parameter<int>("bufferSize", &bufferSize, 7, this, "desc");
    new Variable<double>("value", &value, this, "desc");
}

void SalinityScale::reset() {
    value = 1;
}

void SalinityScale::update() {
    if (salinities.isEmpty()) {
        salinities.fill(Smax, bufferSize);
        index = 0;
    }
    else {
        salinities[index] = Smax;
        index = (index + 1)%bufferSize;
    }

    if (Smax > 10){
        if ((Smax - Smin)>=6)
            value = 0.;
        else {
            if(Smin < 10)
                value = 0.5;
            else
                value = 1.;
        }
    }
    else
        value = 0.;
        }

} //namespace

