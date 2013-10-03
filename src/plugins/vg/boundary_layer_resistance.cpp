/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance.h"

using namespace std;
using namespace UniSim;

namespace vg {

BoundaryLayerResistance::BoundaryLayerResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(windSpeed), "environment[windspeed]");
    addParameterRef<double>(Name(ventilation), "greenhouse/ventilation[value]");
    addVariable<double>(Name(value), "Boundary layer resistance against water vapour (s/m)");
}

void BoundaryLayerResistance::reset() {
    updateValue(0.);
}

void BoundaryLayerResistance::update() {
    updateValue(0.1*windSpeed/4.*ventilation);
}

void BoundaryLayerResistance::updateValue(double windSpeed) {
    // Characteristic leaf dimension
    const double L = 25./1000.;
    // (Stanghellini GCC p 146 modified)
    value = 200.*sqrt( L/max(0.05,windSpeed) );
}

} //namespace

