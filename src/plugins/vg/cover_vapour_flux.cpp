/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/name.h>
#include "cover_vapour_flux.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
CoverVapourFlux::CoverVapourFlux(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "climate/temperature[value]");
    addParameterRef<double>(Name(mcIndoors), "climate/humidity[mc]");
    addParameterRef<double>(Name(diffuseRadiation), "climate/radiation[diffuse]");
    addParameterRef<double>(Name(rbH2O), "crop/rbH2O[value]");
    addVariable<double>(Name(value), "Vapour flux from cover (i.e. glass and screens) (kg/m2/s)");
}

void CoverVapourFlux::reset() {
    value = 0.;
}

void CoverVapourFlux::update() {
    double s = svpSlope(Tindoors);
    double smc = saturatedMoistureContent(Tindoors);
    double absorbedRadiation = 0.66*diffuseRadiation + 0.95*Sigma*pow(1.06*Tindoors-21, 4);
    double effectiveHumidity = smc + RhoAir/Psychr*rbH2O*absorbedRadiation/LHe;
    double transpirationConductance = 1./((1. + s/Psychr)*rbH2O);
    value = transpirationConductance*(effectiveHumidity - mcIndoors)/1000.;
}

} //namespace

