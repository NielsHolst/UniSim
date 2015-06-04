/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DRAINAGELOSSES_H
#define INSECTICIDE_DRAINAGELOSSES_H

#include <usbase/model.h>

namespace insecticide {

class drainagelosses : public UniSim::Model
{
public:
    drainagelosses(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Rt6, Rt3, Rt2, Rt10,
    Rt8, Rt7, Rt9, Rt5,
    Df, Rt1, Rt4, k1, k2, k3, k4,
    k5, k6, k7, k8, k9, k10, Fdd3, Fdd2, Fdd4, Fdd1, Fdd5,
    Fdd6, Fdd7, Fdd8, Fdd9, Fdd10, Doseafl;//Fdd3, Fdd2, Fdd4, Fdd1, Fdd5, Fdd6, Fdd7, Fdd8, Fdd9, Fdd10

    // Output
    double DLosses3, DLosses2, DLosses4, DLosses1,
    DLosses5, DLosses6, DLosses7, DLosses8, DLosses9, DLosses10;
};

} //namespace


#endif
