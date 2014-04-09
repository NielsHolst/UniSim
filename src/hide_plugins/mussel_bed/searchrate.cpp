/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH SEARCH RATE MODEL: This model calculate the search rate parameter for
   the current step conditions of salinity and location specific hydrodynamics*/

#include "searchrate.h"

using namespace UniSim;

namespace mussel_bed {

searchrate::searchrate(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("maxSR",&maxSR, 1, this, "maximum search rate m2/day, from Wong et al, 2006");
    new Variable<double>("value", &value, this, "search rate or appearance");
}

void searchrate::initialize() {
    scales = seekChildren<Model*>("SRscale");
}

void searchrate::reset() {
    value = 0.;
}

void searchrate::update() {
    value = maxSR;
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->pullValue<double>("value");
    }
}


} //namespace
