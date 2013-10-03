/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATION_BY_TEMPERATURE_DIFF_H
#define VG_VENTILATION_BY_TEMPERATURE_DIFF_H

#include <usbase/model.h>

namespace vg {

class VentilationByTemperatureDiff : public UniSim::Model
{
public:
    VentilationByTemperatureDiff(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
//    double windSideRelation, maxTemperature,
//        windSpeed, indoorsTemperature, outdoorsTemperature,
//        ventilationMax, byTemperature, byHumidity, byTemperatureDiff;
    double indoorsTemperature, outdoorsTemperature, ventilationMax;
    // Variables
    double signal;
};
} //namespace


#endif
