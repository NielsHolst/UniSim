/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {

PUBLISH(BoundaryLayerResistance)

BoundaryLayerResistance::BoundaryLayerResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, windSpeed, "outdoors[windspeed]");
    Input(double, ventilation, 1.);
    Output(double, rbH2O); // (s/m)
    Output(double, rbCO2);
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
    rbH2O = 200.*sqrt( L/max(0.05,windSpeed) );
    rbCO2 = rbH2O*1.37;
}

} //namespace

