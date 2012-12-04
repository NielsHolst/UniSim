/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STAGE_AND_PHASE_H
#define UNISIM_STAGE_AND_PHASE_H

#include "distributed_delay_2d.h"
#include "stage_base.h"

namespace UniSim {

class StageAndPhase : public StageBase
{
public:
    StageAndPhase(Identifier name, QObject *parent=0);
    // standard methods
    void reset();
	void update();

    // special methods
    DistributedDelayBase* createDistributedDelay();
    double growthDemand();

private:
	// parameters
    int phaseK;
    double phaseL, timeStep, phaseTimeStep;
    QVector<double> inflow;

    // variables
    QVector<double> outflow, latestInflow, latestPhaseInflow;

    // data
    QVector<double> inflowPending, phaseInflowPending;
    DistributedDelay2D *dd;
};

}
#endif
