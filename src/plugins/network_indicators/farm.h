/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef NETWORK_INDICATORS_FARM_H
#define NETWORK_INDICATORS_FARM_H

#include <usbase/model.h>

namespace network_indicators {

class Farm : public UniSim::Model
{
public:
    Farm(UniSim::Identifier name, QObject *parent);

private:
    // Parameters
    double animalIntake, respiration, soilLosses, uptakeCrops;
};

} //namespace


#endif
