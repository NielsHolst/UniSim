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
    addParameter<double>(Name(spLight), 10.,"Global radiation threshold for using energy screen (W/m2)");
    addParameter<double>(Name(K), 5.2,"K-value (W/m2/K)");
    addVariable<double>(Name(sp),"Set point for energy screen [0;1]");
    addVariable<bool>(Name(isIncreasing),"Is the set point increasing?");
}

void ScreenEnergyBase::initialize() {
    Model *parent = seekParent<Model*>("*");
    type = parent->pullValuePtr<int>("type");
    spMaxOpening = parent->pullValuePtr<double>("spMaxOpening");
    spHumidityPassed = parent->pullValuePtr<bool>("spHumidityPassed");
}

void ScreenEnergyBase::reset() {
    prevSp = -1.;
    update();
}

void ScreenEnergyBase::update() {
    if (*type == 0)
        sp = 0.;
    else
        sp = (useScreen() && *spHumidityPassed) ? *spMaxOpening : 0;
    isIncreasing = (prevSp < 0) ? false : (sp > prevSp);
    prevSp = sp;
}

} //namespace
