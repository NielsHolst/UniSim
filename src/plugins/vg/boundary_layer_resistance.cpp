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
    addParameterRef<double>(Name(windspeed), "outdoors/records[windspeed]");
    addParameterRef<double>(Name(greenhouseVentilation), "greenhouse/ventilation[total]");
    addVariable<double>(Name(rbH2O), "Boundary layer resistance against water vapour (s/m)");
}

void BoundaryLayerResistance::reset() {
    update();
}

void BoundaryLayerResistance::update() {
    // Characteristic leaf dimension
    double l=25/1000;

    /* Determination of wind-speed u in the greenhouse as function of windspeed
       outside the greenhouse (windsp) ventilation through vents (Vntu) and greenhouse
       leakage n*/
    double u = max(0.05, 0.1*windspeed/4*greenhouseVentilation);

     // Boundary layer resistence to water vapour (Stanghellini GCC p 146 modified)
    rbH2O = 200*sqrt(l/u);
  }


} //namespace

