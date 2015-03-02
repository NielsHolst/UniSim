/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_FUNCTIONAL_RESPONSE_G_B
#define UNISIM_FUNCTIONAL_RESPONSE_G_B

#include <usbase/model.h>

namespace UniSim{

class FunctionalResponseGB : public Model
{
	//Q_OBJECT
public:
    FunctionalResponseGB(Identifier name, QObject *parent=0);
    //standard methods
    void reset();
    void update();

private:
    // parameters
    double apparency, resourceDensity, demand,
        egestionRatio, respiration,
        conversionCost;
    bool truncateSupply, costOnRespiration;


    // variables
    double supply, sdRatio, totalDemand, totalSupply, egestion,
        attacksPerHost, numHostsAttacked, propHostsAttacked;
};

} //namespace


#endif
