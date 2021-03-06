/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_FILTERVELOCITY_H
#define INSECTICIDE_FILTERVELOCITY_H

#include <usbase/model.h>

namespace insecticide {

class filtervelocity : public UniSim::Model
{
public:
    filtervelocity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double averageAnnualPrecipitation, averageAnnualIrrigation, potentialAnnualEvaporation,
    ratioofactualevaporationandpotentialevaporation;

    // Output
    double filtervel;
};

} //namespace


#endif
