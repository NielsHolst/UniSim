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
 * - _ventMaxOpening_ is the maximum possible degrees opening  [0;180]
 * - _ventDensity_ is the number of vent windows per greenhouse area [m<SUP>-2</SUP>]
 * - _efficacy_ is the efficacy of ventilation (can be reduced, for example, by insect net) [0;1]
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
    Input(double, ventLength, 3.5);
    Input(double, ventWidth, 0.8);
    Input(double, ventMaxOpening, 45.);
    Input(double, ventDensity, 0.078);
    Input(double, efficacy, 1.);
}


} //namespace

