/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "assimilation_max_given_temp.h"

using namespace UniSim;

namespace intercom{

AssimilationMaxGivenTemp::AssimilationMaxGivenTemp(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable("amax", &amax, this, "description");
}

void AssimilationMaxGivenTemp::initialize()
{
    setParameter("slope", &slope, 0.04, "description");
    setParameter("maxAmax", &maxAmax, 1., "description");
    weather = seekOne<Model*>("weather");
}
void AssimilationMaxGivenTemp::reset() {
    amax = 0.;
}

void AssimilationMaxGivenTemp::update()
{
    double Tday = weather->pullVariable("Tday");
    amax = Tday < 0 ? 0. : slope*Tday;
    if (amax > maxAmax)
        amax = maxAmax;
}

} //namespace

