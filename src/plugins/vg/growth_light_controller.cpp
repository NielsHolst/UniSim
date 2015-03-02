/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/test_num.h>
#include <QList>
#include <QMessageBox>
#include "growth_light_controller.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(GrowthLightController)

/*! \class GrowthLightController
 * \brief Flags whether lights are on or off
 *
 * Inputs
 * ------
 * - None.
 *
 * Outputs
 * ------
 * - _period_ is time lights have been on, or zero if they are switched off  [min]
 * - _signal_ is the on/off signal to lamps [true,false]
 *
 * Default dependencies
 * ------------
 * - zero or more child models named _periods_ with a _flag_ port [true,false]
 * - zero or more child models named _on_ with a _flag_ port [true,false]
 * - zero or more child models named _off_ with a _flag_ port [true,false]
 *
 */

GrowthLightController::GrowthLightController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(bool, signal);
}

void GrowthLightController::amend() {
    periodFlag = getFlag("periods");
    onFlag = getFlag("on");
    offFlag = getFlag("off");
}

const bool * GrowthLightController::getFlag(QString name) {
    auto model = seekOneChild<Model*>(name);
    return model->pullValuePtr<bool>("flag");
}

void GrowthLightController::reset() {
    signal = false;
}

void GrowthLightController::update() {
    signal = *periodFlag && *onFlag && !offFlag;
//    if (!*periodFlag)
//        signal = false;
//    else if (signal && *offFlag)
//        signal = false;
//    else if (!signal && *onFlag)
//        signal = true;
}

} //namespace

