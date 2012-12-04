/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STAGE_H
#define UNISIM_STAGE_H

#include "distributed_delay.h"
#include "stage_base.h"

namespace UniSim {

class Stage : public StageBase
{
public:
    Stage(Identifier name, QObject *parent=0);

    // standard methods
    void reset();
    void update();

    // special methods
    DistributedDelayBase* createDistributedDelay();
    double growthDemand();

private:
	// parameters
    double inflow, initialInflow, phaseOutflowProportion;

    // variables
    double latestInflow, outflow, dt;

    // data
    bool firstUpdate;
    double inflowPending;
    DistributedDelay *dd;
};

}
#endif
