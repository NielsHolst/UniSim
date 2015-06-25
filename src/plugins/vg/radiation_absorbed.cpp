/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "publish.h"
#include "radiation_absorbed.h"

using namespace UniSim;

namespace vg {

PUBLISH(RadiationAbsorbed)

RadiationAbsorbed::RadiationAbsorbed(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, indoorsLight, "indoors/light[total]");
    Output(double, Rna);
}

void RadiationAbsorbed::reset() {
    Rna = 0;
}

void RadiationAbsorbed::update() {
    Rna = indoorsLight*0.66;
}


} //namespace

