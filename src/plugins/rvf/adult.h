/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef ADULT_H
#define ADULT_H

#include <usbase/model.h>

namespace rvf {

class Adult : public UniSim::Model
{
public:
    Adult(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double sexRatio, density, fecundity, proportionEggsInWater;

    // Variables
    double eggsLaidInWater, eggsLaidOnGround;
};

} //namespace

#endif // ADULT_H
