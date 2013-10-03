/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "greenhouse_ventilation.h"

using namespace UniSim;

namespace vg {
	
GreenhouseVentilation::GreenhouseVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addVariable<double>(Name(value), "Total ventilation rate (m3/m2/s");
}

void GreenhouseVentilation::initialize() {
    v.set(this,"greenhouse/ventilation/*[value]");
}

void GreenhouseVentilation::reset() {
    value = 0.;
}

void GreenhouseVentilation::update() {
    value = v.sum();
}

} //namespace

