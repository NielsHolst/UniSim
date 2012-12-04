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
    return  farm->pullValue<double>("uptakeCrops") +
            farm->pullValue<double>("toAnimals") +
            farm->pullValue<double>("toManure") +
            farm->pullValue<double>("fixation") +
            farm->pullValue<double>("deposition") +
            farm->pullValue<double>("nonSymbFixation") +
            farm->pullValue<double>("greenManure") +
            farm->pullValue<double>("totalManure")
            ;
}

} //namespace

