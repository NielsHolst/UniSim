/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TOPSOILDEGRADATION_H
#define INSECTICIDE_TOPSOILDEGRADATION_H

#include <usbase/model.h>

namespace insecticide {

class topsoildegradation : public UniSim::Model
{
public:
    topsoildegradation(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double inflow, k, ff, Q, Tref, Tair,
    Vrsmc, rf, fw, Sp;

    // Output
    double concentration, outflow, Ts, ksd, fsdT, Vsmc, fsm;
};

} //namespace


#endif
