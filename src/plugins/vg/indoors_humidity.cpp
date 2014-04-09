/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_humidity.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
IndoorsHumidity::IndoorsHumidity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(temperature),"indoors/temperature[value]");
    addParameterRef<double>(Name(maxRh),"setpoints/humidity/maximum[setpoint]");
//    addParameterRef<double>(Name(minDeltaX),"setpoints/humidity[minDeltaX]");

    addVariable<double>(Name(rh),"Relative humidity (%)");
    addVariable<double>(Name(vp),"Water vapour pressure (Pa)");
    addVariable<double>(Name(mc),"Moisture content (g/m3)");
    addVariable<double>(Name(vpd),"Water vapour pressure deficit (Pa)");

    addVariable<double>(Name(rhExcess),"Excess of relative humidity cf. setpoint (% points)");
    addVariable<double>(Name(vpdInsufficiency),"Insufficiency of vpd cf. setpoint (Pa)");
    addVariable<bool>(Name(highRh),"Is rh above setpoint?");
    addVariable<bool>(Name(lowDeltaX),"Is vpd below setpoint?");
    addVariable<bool>(Name(highHumidity),"Is rh above of vpd below setpoint?");
}

void IndoorsHumidity::reset() {
    minDeltaX = 0.;
    rh = 80;
    vp = vg::vp(25.,rh);
    mc = vg::moistureContent(25.,vp);
    vpd = vg::vpd(25.,rh);
    setBalances();
}

void IndoorsHumidity::update() {
    rh = vg::rhFromMc(temperature, mc);
    vp = vg::vp(temperature, rh);
    vpd = vg::vpd(temperature, rh);
    setBalances();
}

void IndoorsHumidity::setBalances() {
    rhExcess = rh - maxRh;
    vpdInsufficiency = minDeltaX - vpd;
    highRh = rhExcess > 0.;
    lowDeltaX = vpdInsufficiency > 0.;
    highHumidity = highRh || lowDeltaX;
}

} //namespace
