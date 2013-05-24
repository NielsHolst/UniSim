/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "produsplots.h"

using namespace UniSim;

namespace mussel_bed {

Produsplots::Produsplots(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("stDensity", &stDensity, 0.3, this, "desc"); //initial density of starfish (kg/m2)
    new Parameter<double>("mDensity", &mDensity, 1., this, "desc"); //initial density of mussels (kg/m2)
    new Parameter<QDate>("startDate", &startDate, QDate(2012,1,1), this,
    "Initial date of simulation. "
    "You should perform a @F deepReset on your @F Calendar object after pushing a new "
    "value to @F {initialDate}.");
    new Parameter<double>("days", &days, 30, this, "desc"); //steps for simulation
}


} //namespace
