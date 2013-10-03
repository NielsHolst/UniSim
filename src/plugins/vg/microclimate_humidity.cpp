/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/utilities.h>
#include "general.h"
#include "microclimate_humidity.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
MicroclimateHumidity::MicroclimateHumidity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(timeStepSecs),"calendar[timeStepSecs]");
    addParameterRef<double>(Name(mcOutdoors),"environment[mc]");
    addParameterRef<double>(Name(height),"greenhouse/construction[averageHeight]");
    addParameterRef<double>(Name(ventilation),"greenhouse/ventilation[value]");
    addParameterRef<double>(Name(temperature),"../temperature[value]");
    addParameterRef<double>(Name(mcCropSurface),"crop[surfaceMoistureContent]");
    addParameterRef<double>(Name(cropTranspirationConductance),"crop[transpirationConductance]");
    addParameterRef<double>(Name(screenState),"screens/energy/control[state]");
    addParameterRef<double>(Name(Tscreen),"screens/energy/temperature[value]");
    addParameterRef<double>(Name(Tcover),"greenhouse/cover/temperature[value]");
    addParameterRef<double>(Name(maxRH),"setpoints/humidity[maxRH]");
    addParameterRef<double>(Name(minDeltaX),"setpoints/humidity[minDeltaX]");
    addVariable<double>(Name(rh), "Relative humidity (%)");
    addVariable<double>(Name(prevRh), "Relative humidity (%)");
    addVariable<double>(Name(vp),"Water vapour pressure (Pa)");
    addVariable<double>(Name(vpd),"Water vapour pressure deficit (Pa)");
    addVariable<double>(Name(mc),"Moisture content (g/m3)");
    addVariable<double>(Name(moistureDeficit), "Absolute humidity deficit (g/kg)");
    addVariable<bool>(Name(isHigh),"Is humidity above setpoints?");
}

void MicroclimateHumidity::reset() {
    rh = prevRh = 70.;
    vp = vg::vp(20.,rh);
    vpd = vg::vpd(20.,rh);
    mc = vg::moistureContent(20., vp);
    moistureDeficit = vg::moistureDeficit(20., rh);
    isHigh = false;
}

void MicroclimateHumidity::update() {
    mc = calcMc();
    rh = rhFromMc(temperature, mc);
    vp = vg::vp(temperature, rh);
    vpd = vg::vpd(temperature, rh);
    moistureDeficit = vg::moistureDeficit(temperature, rh);
    isHigh = rh > maxRH || vpd < minDeltaX;
}

double MicroclimateHumidity::calcMc() const {
    // Water vapour conductances (m/s)
    double  gTranspiration = cropTranspirationConductance,
            gVentilation = ventilation, // m3/m2/s
            gCondensation = 3e-3;       // m3/m2 ground/s, corrected for cover/ground ratio

    // Condensation at surfaces
    struct Surface {
        double smc;
        bool isCondensing;
        Surface(double T, double mc) {
            smc = saturatedMoistureContent(T);
            isCondensing = mc > smc;
        }
    };
    Surface screen(Tscreen, mc), cover(Tcover, mc);

    // Water vapour gain of ambient air (m/s*g/m3 = g/m2/s)
    double waterFlux =
            gTranspiration*mcCropSurface
            + gVentilation*mcOutdoors;

    // Sum of conductances (m/s)
    double gTot = gTranspiration + gVentilation;

    // Adding condensation fluxes
    if (screen.isCondensing) {
        double g = gCondensation*screenState;
        waterFlux += g*screen.smc;
        gTot += g;
    }
    if (cover.isCondensing) {
        double g = gCondensation*(1.- screenState);
        waterFlux += g*cover.smc;
        gTot += g;
    }

    // Equilibrium moisture content of greenhouse (g/m3)
    double mcEq = waterFlux/gTot;

    // Integrate new value of mc over time step
    double timeConstant = height/gTot;
    return mcEq - (mcEq - mc)*exp(-timeStepSecs/timeConstant);
}

} //namespace
