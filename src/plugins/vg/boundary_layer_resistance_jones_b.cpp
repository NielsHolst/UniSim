/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance_jones_b.h"
#include "general.h"
#include "publish.h"

using std::max;
using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(BoundaryLayerResistanceJonesB)

/*! \class BoundaryLayerResistanceJonesB
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * According to Jones (1992, eq. A.8.1)
 */

BoundaryLayerResistanceJonesB::BoundaryLayerResistanceJonesB(Identifier name, QObject *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
    InputRef(double, evaporation, "../transpiration[vapourFlux]");
    InputRef(double, leafTemperature, "../temperature[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, indoorsRh, "indoors/humidity[rh]");
}

void BoundaryLayerResistanceJonesB::reset() {
    leafTemperature = indoorsTemperature;
    evaporation = 1e-6;
    BoundaryLayerResistanceBase::reset();
}

void BoundaryLayerResistanceJonesB::setRbH2O() {
    double svpLeaf = svp(leafTemperature),
           vpAir = vpFromRh(indoorsTemperature, indoorsRh),
           vpd = max(svpLeaf - vpAir, 0.),
           gbH2O = max(1000.*evaporation/( 0.622*RhoAir/P0*vpd ), 0.001);

    rbH2O = 1./gbH2O;
    if (std::isnan(rbH2O)) {
        throw Exception("BoundaryLayerResistanceJonesB is not a number");
    }
}

} //namespace

