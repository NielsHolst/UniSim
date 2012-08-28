/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_GROWTH_DEMAND
#define SUPPLYDEMAND_GROWTH_DEMAND
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class GrowthDemand : public UniSim::Model
{
	Q_OBJECT
public: 
    GrowthDemand(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters
    double maxGrowthRate;

    // pull variables
    double value;

    // links
    const double *mass, *growthRate, *duration, *timeStep;
};

} //namespace
#endif
