/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "filtervelocity.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (filtervelocity)
	
filtervelocity::filtervelocity(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, averageAnnualPrecipitation, 25.);     //mm
    Input (double, averageAnnualIrrigation, 2.); //mm
    Input (double, potentialAnnualEvaporation, 20.); // mm
    Input (double, ratioofactualevaporationandpotentialevaporation, 0.1);

    Output (double, filtervel); //filter velocity estimated from annual hydraulic flow (m/d)

}

void filtervelocity::reset() {

    filtervel = 0.;

}

void filtervelocity::update() {

    filtervel = (((averageAnnualPrecipitation + averageAnnualIrrigation)-
                  (potentialAnnualEvaporation * ratioofactualevaporationandpotentialevaporation))*(1./1000.))/365.;

}

} //namespace

