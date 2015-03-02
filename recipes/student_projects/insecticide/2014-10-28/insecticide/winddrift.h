/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_WINDDRIFT_H
#define INSECTICIDE_WINDDRIFT_H

#include <usbase/model.h>

namespace insecticide {

class winddrift : public UniSim::Model
{
public:
    winddrift(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

//Iin shared functions i.e. functions which shares functions, "private" is replaced by "protected".

protected:

    // Functions
    double calcAmountdrifted (double n);

    // Input
    double inflow, P, Wboom, Pass, fieldarea, Ao, A1, Bo, B1;

    // Output
    double fdXpass, Xpass, fdpass, amountdrifted;

    int npass;
};

} //namespace


#endif
