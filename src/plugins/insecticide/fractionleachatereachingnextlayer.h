/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_FRACTIONLEACHATEREACHINGNEXTLAYER_H
#define INSECTICIDE_FRACTIONLEACHATEREACHINGNEXTLAYER_H

#include <usbase/model.h>

namespace insecticide {

class fractionleachatereachingnextlayer : public UniSim::Model
{
public:
    fractionleachatereachingnextlayer(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Rt6, Rt3, Rt2, Rt10,
    Rt8, Rt7, Rt9, Rt5,
    Df, Rt1, Rt4, k1, k2, k3, k4,
    k5, k6, k7, k8, k9, k10, Dp3, Dp2, Dp4, Dp1, Dp5,
    Dp6, Dp7, Dp8, Dp9, Dp10, Doseafl;//Fdd3, Fdd2, Fdd4, Fdd1, Fdd5, Fdd6, Fdd7, Fdd8, Fdd9, Fdd10

    // Output
    double Fn3, Fn2, Fn4, Fn1,
    Fn5, Fn6, Fn7, Fn8, Fn9, Fn10;
};

} //namespace


#endif
