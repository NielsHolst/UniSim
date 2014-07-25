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
    void initialize();
    void reset();
    void update();
private:
    // Input / Output
    double indoorsTemperature, timeStep, averageHeight,
        ah, rh, ahEq, outdoorsAh,
        netVapourFlux, grossVapourFlux,
        timeConstant, surplusAh,
        latentHeatBalance;

    // Data
    struct FluxPtr {
        const double *conductance, *gain, *vapourFlux;
    };
    typedef QVector<FluxPtr> Fluxes;
    Fluxes transpiration, evaporation, condensation, ventilation;
    QVector<const Fluxes*> all;

    // Methods
    void collectFluxes();
    Fluxes collectFluxes(QString fluxName);
    double sumConductance();
    double sumGain();
    double sumVapourFlux(const Fluxes &fluxes);
};

} //namespace

#endif
