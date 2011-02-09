/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "assimilation_max_given_temp.h"

using namespace UniSim;

namespace intercom{

AssimilationMaxGivenTemp::AssimilationMaxGivenTemp(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("radix", &radix, 0.882, this, "description");
    new Parameter<double>("maxAmax", &maxAmax, 50, this, "description");

    new PullVariable<double>("value", &value, this, "Max CO @Sub 2 assimilation rate [kg CO @Sub 2 per ha leaf per hour]");
}

void AssimilationMaxGivenTemp::initialize()
{
    weather = seekOne<Model*>("weather");
}

void AssimilationMaxGivenTemp::reset() {
    value = 0.;
}

void AssimilationMaxGivenTemp::update()
{
    double Tday = weather->pullVariable<double>("Tday");
    value = (Tday < 0.) ? 0. : maxAmax*(1. - pow(radix, Tday));
}

} //namespace

