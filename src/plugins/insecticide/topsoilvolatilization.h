/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TOPSOILVOLATILIZATION_H
#define INSECTICIDE_TOPSOILVOLATILIZATION_H

#include <usbase/model.h>

namespace insecticide {

class topsoilvolatilization : public UniSim::Model
{
public:
    topsoilvolatilization(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double MWair, MVair, Tsa, Pd,
    MV, BLair, fa, fw, farea, Pair,
    Za, Zw, VZ, MW, viscosity;

    // Output
    double Ba, Dw, Dv, Dbl, Bea, Bew, Da, ksv,
    Bw, Dpl, ke;
};

} //namespace


#endif
