/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "transmission_screens.h"

using namespace UniSim;

namespace vg {

PUBLISH(TransmissionScreens)

/*! \class TransmissionScreens
 * \brief Greenhouse light and energy transmission through cover and screens
 */

TransmissionScreens::TransmissionScreens(Identifier name, QObject *parent)
    : TransmissionBase(name, parent)
{
}

bool TransmissionScreens::applyScreens() const {
    return true;
}
} //namespace

