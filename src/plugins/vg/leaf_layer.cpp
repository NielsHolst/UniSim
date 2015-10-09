/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_layer.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(LeafLayer)

/*! \class LeafLayer
 * \brief A crop canopy leaf layer
 *
 * Inputs
 * ------
 * - _xGaussUpperside_ is the LAI coefficient coming from above [0;1]
 * - _wGaussUpperside_ is weighing coefficient coming from above[0;1]
 * - _xGaussLowerside_ is the LAI coefficient coming from below [0;1]
 * - _wGaussLowerside_ is weighing coefficient coming from below[0;1]
 *
 */

LeafLayer::LeafLayer(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, xGaussUpperside, 1);
    Input(double, wGaussUpperside, 1);
    Input(double, xGaussLowerside, 1);
    Input(double, wGaussLowerside, 1);
}



} //namespace

