/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "compartmental_throughflow.h"

using namespace UniSim;

namespace network_indicators {


CompartmentalThroughflow::CompartmentalThroughflow(Identifier name, QObject *parent)
    : IndicatorBase(name, parent)
{
}

double CompartmentalThroughflow::computeIndicator(UniSim::Model *farm) {
    return  rnd(farm->pullValue<double>("uptakeCrops")) +
            rnd(farm->pullValue<double>("toAnimals")) +
            rnd(farm->pullValue<double>("toManure")) +
            rnd(farm->pullValue<double>("fixation")) +
            rnd(farm->pullValue<double>("deposition")) +
            rnd(farm->pullValue<double>("nonSymbFixation")) +
            rnd(farm->pullValue<double>("greenManure")) +
            rnd(farm->pullValue<double>("totalManure"))
            ;
}

} //namespace

