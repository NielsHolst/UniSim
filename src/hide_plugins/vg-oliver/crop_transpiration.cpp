/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "publish.h"
#include "crop_transpiration.h"

using namespace UniSim;

namespace vg {

PUBLISH(CropTranspiration)

CropTranspiration::CropTranspiration(Identifier name, QObject *parent)
    : VapourFlux(name, parent)
{
    InputRef(double, ConductanceIn, "CropConductance[value]");
    InputRef(double, VapourFluxIn, "CropVapourFlux[value]");
    InputRef(double, GainIn, "CropGain[value]");

}

void CropTranspiration::update() {

    conductance = ConductanceIn;
    vapourFlux = VapourFluxIn;
    gain = GainIn;
}


} //namespace

