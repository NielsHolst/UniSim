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
    addParameterRef<double>(Name(globRad), "outdoors/records[globRad]");
    addParameterRef<double>(Name(humidityIncrement), "indoors/temperature/spHeating/humidityControl[response]");
    addParameter<double>(Name(spGlobRad), 300., "Threshold for global radiation dependency (W/m2)");
    addParameter<double>(Name(spHeatingBasis), 20., "Set point for heating (oC)");
    addParameter<double>(Name(spHeatingMax), 24., "Set point for max heating temperature (oC)");
    addVariable<double>(Name(sp), "Setpoint for heating (oC)");
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

