/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "ventilation_for_cooling_top.h"

using namespace UniSim;

namespace vg {

PUBLISH(VentilationForCoolingTop)

/*! \class VentilationForCoolingTop
 * \brief Total air ventilation
 *
 * Inputs
 * ------
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _active_ is the active ventilation [h<SUP>-1</SUP>]
 *
 * Outputs
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 */

VentilationForCoolingTop::VentilationForCoolingTop(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, ventilationForCoolingBottom, "../../bottom/ventilation[value]");
    InputRef(double, gap, "horizontalScreen[gap]");
    Output(double, value);
}

void VentilationForCoolingTop::reset() {
    value = 0;
}

void VentilationForCoolingTop::update() {
    Q_ASSERT(gap>0.);
    value = ventilationForCoolingBottom/gap*(1. - gap);
}

} //namespace

