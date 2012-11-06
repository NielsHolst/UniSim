/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "farm.h"

using namespace UniSim;

namespace network_indicators {
	
Farm::Farm(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("animalIntake", &animalIntake, 0., this, "desc");
    new Parameter<double>("respiration", &respiration, 0., this, "desc");
    new Parameter<double>("soilLosses", &soilLosses, 0., this, "desc");
    new Parameter<double>("uptakeCrops", &uptakeCrops, 0., this, "desc");
}


} //namespace

