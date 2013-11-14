/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef RVF_SHEEPINFECTION_H
#define RVF_SHEEPINFECTION_H

#include <usbase/model.h>

namespace rvf {

class SheepInfection : public UniSim::Model
{
public:
    SheepInfection(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double infectionRate, duration, mortalityRate, exposureRate, density;

    // Variables
    double mortality;

    //Links

};

} //namespace

#endif // RVF_MOSQUITODEMAND_H

