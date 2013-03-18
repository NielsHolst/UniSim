/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef NETWORK_INDICATORS_COMPARTMENTAL_THROUGHFLOW_H
#define NETWORK_INDICATORS_COMPARTMENTAL_THROUGHFLOW_H

#include "indicator_base.h"

namespace network_indicators {

class CompartmentalThroughflow : public IndicatorBase
{
public:
    CompartmentalThroughflow(UniSim::Identifier name, QObject *parent);
private:
    // methods
    double computeIndicator(UniSim::Model *farm);
};

} //namespace


#endif
