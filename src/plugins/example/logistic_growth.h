/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef EXAMPLE_LOGISTIC_GROWTH_H
#define EXAMPLE_LOGISTIC_GROWTH_H

#include <usbase/model.h>

namespace example {

class LogisticGrowth : public UniSim::Model
{
public:
    LogisticGrowth(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    double initialDensity, growthRate, carryingCapacity, timeStep,
        density;
};

} //namespace


#endif
