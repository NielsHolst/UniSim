/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "construction_floor.h"
#include "publish.h"

using std::tan;
using std::hypot;
using namespace UniSim;

namespace vg {
	
PUBLISH(ConstructionFloor)

/*! \class ConstructionFloor
 * \brief Defines the construction details of the greenhouse floor
 *
 * Inputs
 * ------
 * - _U_ is the heat transfer coefficient of the material [W/m<SUP>2</SUP>/K]
 *
 * Outputs
 * -------
 * - none

 * Default dependencies
 * ------------
 * - none
 */

ConstructionFloor::ConstructionFloor(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, U, 6.);
}


} //namespace

