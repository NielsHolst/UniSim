/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_WINDDRIFT_H
#define INSECTICIDE_WINDDRIFT_H

#include <usbase/model.h>

namespace insecticide {

class WindDrift : public UniSim::Model
{
public:
    WindDrift(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

protected:
    // Functions
    double calcAmountdrifted(double n);

    // Input
    double inflow, P, Wboom, Pass, A;

    // Output
    double fdXpass, Xpass, fdpass, amountdrifted;
};

} //namespace


#endif
