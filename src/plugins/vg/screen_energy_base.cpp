/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_energy_base.h"

using namespace UniSim;

namespace vg {
	
ScreenEnergyBase::ScreenEnergyBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("spLight", &spLight, 10., this,"Global radiation threshold for using energy screen (W/m2)");
    new Variable<double>("sp", &sp, this,"Set point for energy screen [0;1]");
}

void ScreenEnergyBase::initialize() {
    Model *parent = seekParent<Model*>("*");
    type = parent->pullValuePtr<int>("type");
    spMaxOpening = parent->pullValuePtr<double>("spMaxOpening");
    spHumidityPassed = parent->pullValuePtr<bool>("spHumidityPassed");
}

void ScreenEnergyBase::reset() {
    update();
}

void ScreenEnergyBase::update() {
    if (*type == 0)
        sp = 0.;
    else
        sp = (useScreen() && *spHumidityPassed) ? *spMaxOpening : 0;
}

} //namespace
