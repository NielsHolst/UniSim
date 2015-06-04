/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATION_BY_COOLING_H
#define VG_VENTILATION_BY_COOLING_H

#include <usbase/model.h>

namespace vg {

class VentilationByCooling : public UniSim::Model
{
public:
    VentilationByCooling(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs;
    double energyFlux, indoorsTemperature, outdoorsTemperature, averageHeight;
    // Outputs
    double value;
};
} //namespace


#endif
