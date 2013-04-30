/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sp_ventilation_rh.h"

using namespace UniSim;

namespace vg {
	
SpVentilationRh::SpVentilationRh(Identifier name, QObject *parent)
    : SpVentilationBase(name, parent)
{
    new Parameter<double>("pBand", &pBand, 5, this, "Slope for max decrement due to humidity");
    new Parameter<double>("spRh", &spRh, 85, this, "Setpoint for relative humidity (%)");
    new Parameter<double>("rh", &rh, 80, this, "Indoors humidity (%)");
}

double SpVentilationRh::spDecrement() {
    double deltaRh = spRh - rh;
    return propControl(deltaRh, pBand, maxDecrement);
}

} //namespace

