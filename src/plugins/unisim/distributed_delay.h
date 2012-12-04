/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DISTRIBUTED_DELAY_H
#define UNISIM_DISTRIBUTED_DELAY_H

#include "distributed_delay_base.h"

namespace UniSim {

class DistributedDelay : public DistributedDelayBase
{
public:
    struct Parameters {
        double L;
        int k, minIter;
    };
    struct State {
//        const double *data;
        double outflowRate, growthRate;
        State() : outflowRate(0), growthRate(0) {}
    };
    struct Brackets {
        double fgr1, fgr2, growth1, growth2;
    };

    DistributedDelay(const Parameters &p, QObject *parent);
    DistributedDelay(const DistributedDelay &dd);
    void update(double inflow, double dt, double fgr);
    State state() const;

    Brackets bracket(double inflow, double dt, double fgr, double sdRatio);
    double findFgr(double inflow, double dt, double fgr, double sdRatio);

private:
    // data
    Parameters p;
    State s;
};

}
#endif
