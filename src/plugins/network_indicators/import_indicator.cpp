/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "import_indicator.h"

using namespace UniSim;

namespace network_indicators {
	
ImportIndicator::ImportIndicator(Identifier name, QObject *parent)
    : IndicatorBase(name, parent)
{
}

double ImportIndicator::computeIndicator(UniSim::Model *farm) {
    double dep = rnd(farm->pullValue<double>("deposition"));

    return  dep +
            rnd(farm->pullValue<double>("nonSymbFixation")) +
            rnd(farm->pullValue<double>("importCrops")) +
            rnd(farm->pullValue<double>("fixation")) +
            rnd(farm->pullValue<double>("importManure"))
            ;
}

} //namespace

