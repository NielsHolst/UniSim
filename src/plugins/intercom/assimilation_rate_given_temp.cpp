/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "assimilation_rate_given_temp.h"

using namespace UniSim;

namespace intercom{

AssimilationRateGivenTemp::AssimilationRateGivenTemp(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("rate", &rate);
}

void AssimilationRateGivenTemp::initialize()
{
    new Parameter<double>("slope", &slope, -0.00095);
    new Parameter<double>("intercept", &intercept, 0.0635);
    weather = seekOne<Model*>("weather");
}
void AssimilationRateGivenTemp::reset() {
    rate = 0.;
}

void AssimilationRateGivenTemp::update()
{
    double Tday = weather->state("Tday");
    rate = slope*Tday + intercept;
}

} //namespace

