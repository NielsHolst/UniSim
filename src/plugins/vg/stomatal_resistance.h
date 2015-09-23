/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_STOMATAL_RESISTANCE_H
#define VG_STOMATAL_RESISTANCE_H

#include <usbase/model.h>

namespace vg {

class StomatalResistance : public UniSim::Model
{
public:
    StomatalResistance(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double rh, rbCO2, co2, Pg;
    // Outputs
    double rsH2O, rsCO2;
    // Methods
    void updateFromPg(double Pg);
};
} //namespace


#endif
