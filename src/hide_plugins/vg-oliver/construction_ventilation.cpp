/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/exception.h>
#include "construction_ventilation.h"
#include "publish.h"

using std::tan;
using std::hypot;
using namespace UniSim;

namespace vg {
	
PUBLISH(ConstructionVentilation)

/*! \class ConstructionVentilation
 * \brief Defines the construction details of the greenhouse vents
 *
 * Inputs
 * ------
 * - _ventLength_ is the length of a vents window [m]
 * - _ventWidth_ is the width of a vents window [m]
 * - _ventMaxOpening_ is the maximum possible opening [0;180]
 * - _ventDensity_ is the number of vent windows per greenhouse area [m<SUP>-2</SUP>]
 *
 * Outputs
 * -------
 * - none

 * Default dependencies
 * ------------
 * - none
 */

ConstructionVentilation::ConstructionVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, ventLength, 2.85);
    Input(double, ventWidth, 1.);
    Input(double, ventMaxOpening, 45.);
    Input(double, ventDensity, 0.078);
}


} //namespace

