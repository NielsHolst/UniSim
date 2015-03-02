/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIPAPHIS_DEMAND_H
#define LIPAPHIS_DEMAND_H

#include <usbase/model.h>

namespace lipaphis {

class LipaphisDemand : public UniSim::Model
{
public:
    LipaphisDemand(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    double totalDemand, demandRate, numPredators, timeStep;
};

} //namespace


#endif
