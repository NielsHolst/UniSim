/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <usbase/exception.h>
#include "publish.h"
#include "vent.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(Vent)

/*! \class Vent
 * \brief Calculates air exchange through vents
 *
 * Inputs
 * ------
 * - _length_ is the length of a vents window [m]
 * - _width_ is the width of a vents window [m]
 * - _number_ is the number of vent windows [N]
 * - _porosity_ is the efficacy of ventilation (can be reduced, for example, by insect net) [0;1]
 *
 * Outputs
 * ------
 * - _effectiveArea_ is the total area of the vents corrected for porosity [m<SUP>2</SUP>]

 */

Vent::Vent(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, length, 3.5);
    Input(double, height, 0.8);
    Input(double, number, 10);
    Input(double, porosity, 1.);
    Output(double, effectiveArea);
}

void Vent::reset() {
    effectiveArea = length*height*number*porosity;
}
} //namespace


