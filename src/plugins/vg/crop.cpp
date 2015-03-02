/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(Crop)

/*! \class Crop
 * \brief Crop growth, development and microclimate
 *
 * Inputs
 * ------
 * - _density_ is the number of plants per ground area [m<SUP>-2</SUP>]
 * - _fractionPlantArea_ is the fraction of ground area covered by the crop [0;1]
 *
 * Output
 * ------
 *
 * Default dependencies
 * ------------
 */

Crop::Crop(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, density, 1.);
    Input(double, fractionPlantArea, 1.);
}


} //namespace

