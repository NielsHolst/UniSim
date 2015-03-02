/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_LEAFVOLATILIZATION_H
#define INSECTICIDE_LEAFVOLATILIZATION_H

#include <usbase/model.h>

namespace insecticide {

class leafvolatilization : public UniSim::Model
{
public:
    leafvolatilization(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double VP, Ea, R, Tr, Ta, Doseappl;

    // Output
    double ln_ER, ER, fv, fr, kTr, klv;


};

} //namespace


#endif
