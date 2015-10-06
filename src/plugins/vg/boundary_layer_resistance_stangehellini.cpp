/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance_stangehellini.h"
#include "general.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {

PUBLISH(BoundaryLayerResistanceStangehellini)

/*! \class BoundaryLayerResistanceJones
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Acording to Stangehellini (1987, p. 32)
 */

BoundaryLayerResistanceStangehellini::BoundaryLayerResistanceStangehellini(Identifier name, QObject *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
    InputRef(double, leafTemperature, "../temperature[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
}

void BoundaryLayerResistanceStangehellini::setRbH2O() {
    double dTemp = fabs(leafTemperature - indoorsTemperature);
    rbH2O = 1174*sqrt(leafDimension)/pow(leafDimension*dTemp + 207*sqr(leafWindSpeed), 0.25);
}

} //namespace

