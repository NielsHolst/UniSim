/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sp_heating.h"

using namespace UniSim;

namespace vg {
	
SpHeating::SpHeating(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("globRad", &globRad, 0., this, "Global radiation (W/m2)");
    new Parameter<double>("spGlobRad", &spGlobRad, 300., this, "Threshold for global radiation dependency (W/m2)");
    new Parameter<double>("spHeatingBasis", &spHeatingBasis, 20., this, "Set point for heating (oC)");
    new Parameter<double>("spHeatingMax", &spHeatingMax, 24., this, "Set point for max heating temperature (oC)");
    new Parameter<double>("humidityIncrement", &humidityIncrement, 0., this, "Set point increment due to humidity (oC)");
    new Variable<double>("sp", &sp, this, "Setpoint for heating (oC)");
}

void SpHeating::reset() {
    update();
}

void SpHeating::update() {
    // Global radiation influence
    if (globRad > spGlobRad) {
        double spDif = spHeatingMax - spHeatingBasis;
        sp = spHeatingBasis + spDif*(1-exp((-0.01*globRad)/spDif));
    }
    else
        sp = spHeatingBasis;

    // Humidity influence
    sp += humidityIncrement;

    // Morning influence to avoid cold-fall from screens
    if (sp > spHeatingMax)
        sp = spHeatingMax;
}

} //namespace

