/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_INDOORS_HUMIDITY_H
#define VG_INDOORS_HUMIDITY_H

#include <usbase/model.h>

namespace vg {

class IndoorsHumidity : public UniSim::Model
{
public:
    IndoorsHumidity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    struct Flux {
        double transpiration, condensation, ventilation;
        double sum() { return transpiration + condensation + ventilation; }
    };
    Flux conductance, gain, grossFlux;
    double indoorsTemperature, timeStep, averageHeight,
        ah, rh, ahEq, rhEq, outdoorsAh,
        netVapourFlux, grossVapourFlux,
        timeConstant, extraCondensation, extraVentilation,
        latentHeatBalance;

//    // Variable
//    double rh,
//        rhExcess, wvpdInsufficiency;
//    bool highRh, lowDeltaX, highHumidity;

    // Methods
    void updateFromMc();
};

} //namespace

#endif
