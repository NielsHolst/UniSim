/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "leaf_lai.h"
#include "general.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(LeafLai)


/*! \class LeafLai
 * \brief
 *
 * Inputs
 * ------

 * Output
 * ------
 *
 */

LeafLai::LeafLai(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, xGaussUpperside, "../../..[xGaussUpperside]");
    InputRef(double, wGaussUpperside, "../../..[wGaussUpperside]");
    InputRef(double, lai, "crop/lai[value]");
    Output(double, value);
}
void LeafLai::reset() {
    update();
}

void LeafLai::update() {
    value = lai*xGaussUpperside*wGaussUpperside;
}


} //namespace

