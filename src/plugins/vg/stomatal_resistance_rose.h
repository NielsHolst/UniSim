/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_STOMATAL_RESISTANCE_ROSE_H
#define VG_STOMATAL_RESISTANCE_ROSE_H

#include "stomatal_resistance_base.h"

namespace vg {

class StomatalResistanceRose : public StomatalResistanceBase
{
public:
    StomatalResistanceRose(UniSim::Identifier name, QObject *parent);

private:
    // Inputs
    double rh, rbCO2, co2, Pn, lai;
    // Methods
    double resetRsH2O();
    double updateRsH2O();
    double calcRsH2O(double A);
};
} //namespace


#endif
