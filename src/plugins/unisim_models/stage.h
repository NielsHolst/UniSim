/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STAGE_H
#define UNISIM_STAGE_H

#include <QObject>
#include <QVector>
#include <usbase/model.h>
#include "distributed_delay.h"

namespace UniSim {

class Stage : public UniSim::Model
{
	Q_OBJECT
public:
    Stage(Identifier name, QObject *parent=0);
    ~Stage();
    // standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    const double *data();
    double growthDemand();

private:
	// parameters
    bool isDemandDriven;
    double L, fgr, initialInflow;
    int k;

    // pull variables
    double dt;
    bool firstUpdate;
    double sum, latestInflow, inflowTotal, outflow, outflowTotal, growth;

    // push variables
    double inflow, sdRatio, instantMortality;

    // links
    Model *time;

	// methods
    void applyInstantMortality();

    // data
    double inflowPending;
    DistributedDelay *dd;
};

}
#endif
