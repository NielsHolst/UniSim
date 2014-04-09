/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fresponse_stscale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

FresponseStScale::FresponseStScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("mdensity", &mdensity, 1., this, "desc");
    new Variable<double>("value", &value, this, "desc");
}

void FresponseStScale::reset() {
    value = 1;
}

void FresponseStScale::update() {
    if (mdensity > 0)
        value = 0.1867*log(mdensity)+0.5701;
    else
        value = 0.;
    }

} //namespace
