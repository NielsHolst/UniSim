/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_HAZARDQUOTIENT_H
#define INSECTICIDE_HAZARDQUOTIENT_H

#include <usbase/model.h>

namespace insecticide {

class hazardquotient : public UniSim::Model
{
public:
    hazardquotient(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double RV,lowestAvailableNOAEL, concentration;

    // Output
    double ReferenceValue,HQ;
};

} //namespace


#endif
