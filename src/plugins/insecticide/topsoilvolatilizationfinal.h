/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TOPSOILVOLATILIZATIONFINAL_H
#define INSECTICIDE_TOPSOILVOLATILIZATIONFINAL_H

#include <usbase/model.h>

namespace insecticide {

class topsoilvolatilizationfinal : public UniSim::Model
{
public:
    topsoilvolatilizationfinal(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Rainfall, threshold, ksv, inflow, Va, Za, Vw, Zw, VZ;

    // Output
    double  fv, concentration, outflow, frsw, frsa, svDoseaflrm, Dosetsv;

};

} //namespace


#endif
