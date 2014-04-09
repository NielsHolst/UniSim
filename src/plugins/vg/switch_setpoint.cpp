/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "switch_setpoint.h"

using namespace UniSim;

namespace vg {
	
SwitchSetpoint::SwitchSetpoint(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<bool>(Name(on), "./timeSwitch[timeOn]");
    addParameter<double>(Name(setpoint), 0., "Setpoint value");
}


} //namespace

