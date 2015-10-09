/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "proportional_signal.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(ProportionalSignal)

/*! \class ProportionalSignal
 * \brief Provides proportional control towards a signal
 *
 * For further details, see SlidingSignal
 *
 */

ProportionalSignal::ProportionalSignal(Identifier name, QObject *parent)
    : SlidingSignal(name, parent)
{
}

double ProportionalSignal::slide(double proportion) {
    return proportion;
}


} //namespace

