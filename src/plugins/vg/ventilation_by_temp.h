/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATION_BY_TEMP_H
#define VG_VENTILATION_BY_TEMP_H

#include <usbase/model.h>

namespace vg {

class VentilationByTemp : public UniSim::Model
{
public:
    VentilationByTemp(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double indoorsTemperature, outdoorsTemperature, windowLength, windowHeight, greenhouseArea,
        dischargeCoefficient;
    // Output
    double value;
};
} //namespace


#endif
