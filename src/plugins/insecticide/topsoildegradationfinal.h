/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TOPSOILDEGRADATIONFINAL_H
#define INSECTICIDE_TOPSOILDEGRADATIONFINAL_H

#include <usbase/model.h>

namespace insecticide {

class topsoildegradationfinal : public UniSim::Model
{
public:
    topsoildegradationfinal(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double inflow, k, ff, Q, Tref, Tair, P, threshold, timeSinceLastRainEvent,
    Vrsmc, rf, fw, Sp;

    // Output
    double concentration, outflow, Ts, ksd, fsdT, Vsmc, fsm, Dosesd, sdDoseaflrm;

};

} //namespace


#endif
