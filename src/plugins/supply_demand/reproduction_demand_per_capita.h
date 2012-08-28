/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_REPRODUCTION_DEMAND_PER_CAPITA
#define SUPPLYDEMAND_REPRODUCTION_DEMAND_PER_CAPITA
#include <QObject>
#include "reproduction_demand.h"

namespace SupplyDemand {

class ReproductionDemandPerCapita : public ReproductionDemand
{
	Q_OBJECT
public: 
    ReproductionDemandPerCapita(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void update();

private:
	// parameters
    double eggProduction;

};

} //namespace
#endif
