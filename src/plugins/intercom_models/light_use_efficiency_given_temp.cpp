/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "light_use_efficiency_given_temp.h"

using namespace UniSim;

namespace intercom{

LightUseEfficiencyGivenTemp::LightUseEfficiencyGivenTemp(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("slope", &slope, -0.00095, this, "description");
    new Parameter<double>("intercept", &intercept, 0.0635, this, "description");
    new PullVariable<double>("efficiency", &efficiency, this, "description");
}

void LightUseEfficiencyGivenTemp::initialize()
{
    weather = seekOne<Model*>("weather");
}
void LightUseEfficiencyGivenTemp::reset() {
    efficiency = 0.;
}

void LightUseEfficiencyGivenTemp::update()
{
    double Tday = weather->pullVariable<double>("Tday");
    efficiency = slope*Tday + intercept;
}

} //namespace

