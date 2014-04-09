/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##SALINITY SCALE FOR MUSSEL GROWTH: scale mussel growth with the effect of the current step daily average salinity.
   formulation is the result of field data analysis within the Wadden Sea (PRODUS project experimental plots)*/

#include "salinity_scale.h"

using namespace UniSim;

namespace mussel_bed {

SalinityScale::SalinityScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("Smax", &Smax, 30., this, "salinity at high tide");
    new Parameter<double>("Smin", &Smin, 24., this, "salinity at low tide");
    new Variable<double>("value", &value, this, "mussel growth scaling factor");
}

void SalinityScale::reset() {
    value = 1;
}

void SalinityScale::update() {
    double range = Smax - Smin;
    value = exp(3.69365-2.15219*log(range))/(1+exp(3.69365-2.15219*log(range)));
}
} //namespace

