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
    addVariable<double>(Name(spRh), "Setpoint for relative humidity (%)");
    addVariable<double>(Name(spDeltaX), "Setpoint for delta x (%)");
}


} //namespace

