/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "indoors_ventilation.h"

using namespace UniSim;

namespace vg {
	
IndoorsVentilation::IndoorsVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addVariable<double>(Name(value), "Total ventilation rate (m3/h");
}

void IndoorsVentilation::amend() {
    v.set(this,"../*[value]");
}

void IndoorsVentilation::reset() {
    value = 0.;
}

void IndoorsVentilation::update() {
    value = v.sum();
}

} //namespace

