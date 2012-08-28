/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "light_use_efficiency_given_temp.h"

using namespace UniSim;

namespace intercom{

LightUseEfficiencyGivenTemp::LightUseEfficiencyGivenTemp(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("slope", &slope, -0.0095, this, "Slope of the line (kg CO @Sub 2 per ha leaf per hour per (W per m @Sup 2 leaf) per {@Degree}C)");
    new Parameter<double>("intercept", &intercept, 0.635, this, "Intercept of the line (kg CO @Sub 2 per ha leaf per hour per (W per m @Sup 2 leaf)')");

    new Variable<double>("value", &value, this, "Light use efficiency, @Sym epsilon (kg CO @Sub 2 per ha leaf per hour per (W per m @Sup 2 leaf)')");
}

void LightUseEfficiencyGivenTemp::initialize()
{
    weather = seekOne<Model*>("weather");
}

void LightUseEfficiencyGivenTemp::reset() {
    value = 0.;
}

void LightUseEfficiencyGivenTemp::update()
{
    double Tday = weather->pullValue<double>("Tday");
    value = slope*Tday + intercept;
}

} //namespace

