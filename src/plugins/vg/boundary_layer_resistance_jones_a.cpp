/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance_jones_a.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {

PUBLISH(BoundaryLayerResistanceJonesA)

/*! \class BoundaryLayerResistanceJonesA
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * According to Jones (1992, eq. 3.31)
 */

BoundaryLayerResistanceJonesA::BoundaryLayerResistanceJonesA(Identifier name, QObject *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
}

void BoundaryLayerResistanceJonesA::setRbH2O() {
    double gbH2O = 6.62*sqrt(leafWindSpeed/leafDimension),
           rbH2Olaminar = 1000./gbH2O,
           fTurbulence = 1.5;

    rbH2O = rbH2Olaminar/fTurbulence;
}

} //namespace

