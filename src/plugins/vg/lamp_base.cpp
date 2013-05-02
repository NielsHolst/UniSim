/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lamp_base.h"

using namespace UniSim;

namespace vg {
	
LampBase::LampBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<bool>(Name(switchedOn), "lamps/switch[on]");
    addParameter<double>(Name(capacity), 40., "Capacity of installed assimilation lamps (W/m2)");
    addVariable<double>(Name(convectiveHeat), "Convective heat (W/m2)");
    addVariable<double>(Name(bnLight), "Longwave radiation exchanged from the canopy (W m-2)");
    addVariable<double>(Name(rnLight), "Shortwave radiation reaching the canopy (W/m2)");
    addVariable<double>(Name(par), "PAR that reaches the crop");
    addVariable<double>(Name(energyUse), "Energy use, including the ballast");
}

void LampBase::reset() {
    setToZero();
}

void LampBase::setToZero() {
    convectiveHeat = bnLight = rnLight = par = energyUse = 0.;
}

void LampBase::update() {
    if (switchedOn)
        calculate();
    else
        setToZero();
}



} //namespace

