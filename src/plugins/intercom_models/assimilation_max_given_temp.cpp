/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "assimilation_max_given_temp.h"

using namespace UniSim;

namespace intercom{

AssimilationMaxGivenTemp::AssimilationMaxGivenTemp(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("amax", &amax);
}

void AssimilationMaxGivenTemp::initialize()
{
    setParameter("slope", &slope, 0.04);
    setParameter("maxAmax", &maxAmax, 1.);
    weather = seekOne<Model*>("weather");
}
void AssimilationMaxGivenTemp::reset() {
    amax = 0.;
}

void AssimilationMaxGivenTemp::update()
{
    double Tday = weather->state("Tday");
    amax = Tday < 0 ? 0. : slope*Tday;
    if (amax > maxAmax)
        amax = maxAmax;
}

} //namespace

