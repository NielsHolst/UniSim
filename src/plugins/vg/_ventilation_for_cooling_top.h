/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATION_FOR_COOLING_TOP_H
#define VG_VENTILATION_FOR_COOLING_TOP_H

#include <usbase/model.h>

namespace vg {

class VentilationForCoolingTop : public UniSim::Model
{
public:
    VentilationForCoolingTop(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs;
    double ventilationForCoolingBottom, gap;
    // Outputs
    double value;
};
} //namespace


#endif
