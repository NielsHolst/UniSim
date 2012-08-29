/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_REPRODUCTION_DEMAND_SIZE_DEPENDENT
#define SUPPLYDEMAND_REPRODUCTION_DEMAND_SIZE_DEPENDENT
#include <QObject>
#include "reproduction_demand.h"

namespace SupplyDemand {

class ReproductionDemandSizeDependent : public ReproductionDemand
{
	//Q_OBJECT
public: 
    ReproductionDemandSizeDependent(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void update();

private:
	// parameters
    double massThreshold, eggProduction;

    // links
    const double *individualMass;
};

} //namespace
#endif
