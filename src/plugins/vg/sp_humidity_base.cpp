/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "sp_humidity_base.h"

using namespace UniSim;

namespace vg {
	
SpHumidityBase::SpHumidityBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Variable<double>("spRh", &spRh, this, "Setpoint for relative humidity (%)");
    new Variable<double>("spDeltaX", &spDeltaX, this, "Setpoint for delta x (%)");
}


} //namespace

