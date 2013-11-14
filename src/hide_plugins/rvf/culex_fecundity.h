/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RVF_CULEX_FECUNDITY_H
#define RVF_CULEX_FECUNDITY_H

#include <usbase/model.h>

namespace rvf {

class CulexFecundity : public UniSim::Model
{
public:
    CulexFecundity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double waterLevel, waterLevelThreshold, dailyFecundity, sexRatio, density;

    // Variables
    double value;
};

} //namespace


#endif
