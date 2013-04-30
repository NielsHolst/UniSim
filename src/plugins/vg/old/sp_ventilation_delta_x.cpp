/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sp_ventilation_delta_x.h"

using namespace UniSim;

namespace vg {
	
SpVentilationDeltaX::SpVentilationDeltaX(Identifier name, QObject *parent)
    : SpVentilationBase(name, parent)
{
    new Parameter<double>("pBand", &pBand, 0.5, this, "Slope for max decrement due to humidity (oC)");
    new Parameter<double>("spDeltaX", &spDeltaX, 1, this, "Setpoint for delta x (g/m3)");
    new Parameter<double>("moistureDeficit", &moistureDeficit, 2, this, "Moisture deficit (g/m3)");
}

double SpVentilationDeltaX::spDecrement() {
    double deltaRX = spDeltaX - moistureDeficit;
    return propControl(deltaRX, pBand, maxDecrement);
}


} //namespace

