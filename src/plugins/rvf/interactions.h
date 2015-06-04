/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef RVF_INTERACTIONS_H
#define RVF_INTERACTIONS_H

#include <usbase/model.h>

namespace rvf {

class Interactions : public UniSim::Model
{
public:
    Interactions(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters

    // Variables
    double susceptibleAedesBloodmeals, susceptibleCulexBloodmeals, infectiousAedesBloodmeals, infectiousCulexBloodmeals,
    susceptibleLambsAttackedByInfectiousMosquitoesProportion, susceptilbeAdultSheepAttackedByInfectiousMosquitoesProportion,
    susceptibleAedesOnInfectiousHostBloodmealsProportion, susceptibleCulexOnInfectiousHostBloodmealsProportion;

    //Links
    UniSim::Model *infection, *culexSAdult, *aedesSAdult, *sheepSAdult, *sheepSLamb;
};

} //namespace

#endif // RVF_INTERACTIONS_H
