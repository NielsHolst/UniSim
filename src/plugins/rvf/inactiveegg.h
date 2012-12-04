/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef INACTIVEEGG_H
#define INACTIVEEGG_H

#include <usbase/model.h>


namespace rvf {

class InactiveEgg: public UniSim::Model
{
public:
    InactiveEgg(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double initialDensity, lossRate, activationRate, inflow;

    // Variables
    double density, eggsLost, eggsActivated;
};

} //namespace


#endif // INACTIVEEGG_H
