/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "fixed_signal.h"

using namespace UniSim;

namespace vg {

PUBLISH(FixedSignal)

/*! \class FixedSignal
 * \brief A signal equal to the input
 *
 *
 */

FixedSignal::FixedSignal(Identifier name, QObject *parent)
    : BaseSignal(name, parent){
    Input(double, input, 0.);
}

double FixedSignal::signal() {
    return input;
}

} //namespace

