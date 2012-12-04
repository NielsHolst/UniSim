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
    return  farm->pullValue<double>("deposition") +
            farm->pullValue<double>("nonSymbFixation") +
            farm->pullValue<double>("importCrops") +
            farm->pullValue<double>("fixation") +
            farm->pullValue<double>("importManure")
            ;
}

} //namespace

