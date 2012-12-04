/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STAGE_BASE_H
#define UNISIM_STAGE_BASE_H

#include <QObject>
#include <QVector>
#include <usbase/model.h>
#include "distributed_delay_base.h"

namespace UniSim {

class StageBase : public Model
{
	//Q_OBJECT
public:
    StageBase(Identifier name, QObject *parent=0);
    ~StageBase();
    // standard methods
	void initialize();
	void reset();

    // special methods
    const double *data();
    virtual DistributedDelayBase* createDistributedDelay() = 0;
    virtual double growthDemand() = 0;

protected:
	// parameters
    int k;
    double L, fgr, sdRatio, instantMortality, instantLossRate;
    QVector<double> phaseInflow;

    // variables
    double sum, inflowTotal, outflowTotal, phaseInflowTotal, phaseOutflowTotal, growth;
    QVector<double> phaseOutflow;

    // links
    Model *time;

	// methods
    void applyInstantMortality();

private:
    DistributedDelayBase *ddBase;
};

}
#endif
