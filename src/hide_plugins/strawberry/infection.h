/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STRAWBERRY_INFECTION_H
#define STRAWBERRY_INFECTION_H

#include <usbase/model.h>

namespace strawberry {

class Infection : public UniSim::Model
{
public:
    Infection(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    double inoculumDensity, infectionRate;
    QString sourceModelName, targetModelName;

    // Variables
    double leavesInfected;

    // Links
    UniSim::Model *sourceModel, *targetModel;
};

} //namespace


#endif
