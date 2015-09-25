/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_STOMATAL_RESISTANCE_TOMATO_H
#define VG_STOMATAL_RESISTANCE_TOMATO_H

#include "stomatal_resistance_base.h"

namespace vg {

class StomatalResistanceTomato : public StomatalResistanceBase
{
public:
    StomatalResistanceTomato(UniSim::Identifier name, QObject *parent);

private:
    // Inputs
    double riH2Omin, lai, temperature, radiationAbsorbed,
        rh, co2, indoorsTemperature;
    // Methods
    double resetRsH2O();
    double updateRsH2O();
    double calcRsH2O(double A);
};
} //namespace


#endif
