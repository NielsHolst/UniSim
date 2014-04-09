/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef EVEN_ODD_H
#define EVEN_ODD_H

#include <usbase/model.h>

namespace coral_reef {

class Spawning : public UniSim::Model
{
public:
    Spawning(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    int actualDay, spawningDay;
    double noOfFish, fecundity;

    // Variables
    double noOfEggs;

    // Data
    int prevActualDay;
};

} //namespace


#endif
