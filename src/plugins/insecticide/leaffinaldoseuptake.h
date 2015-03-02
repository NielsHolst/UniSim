/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_LEAFFINALDOSEUPTAKE_H
#define INSECTICIDE_LEAFFINALDOSEUPTAKE_H

#include <usbase/model.h>

namespace insecticide {

class leaffinaldoseuptake : public UniSim::Model
{
public:
    leaffinaldoseuptake(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double klu, kov, inflow, P, threshold;

    // Output
    double Doselfdu, concentration, outflow;
};

} //namespace


#endif
