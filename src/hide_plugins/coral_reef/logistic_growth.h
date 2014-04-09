/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CORAL_REEF_EVEN_ODD_H
#define CORAL_REEF_EVEN_ODD_H

#include <usbase/model.h>

namespace coral_reef {

class LogisticGrowth : public UniSim::Model
{
public:
    LogisticGrowth(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double initialDensity, growthRate, carryingCapacity;
    int timeStep;

    // Variables
    double density;
};

} //namespace


#endif
