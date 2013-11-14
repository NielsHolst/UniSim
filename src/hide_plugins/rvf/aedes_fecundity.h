/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RVF_AEDES_FECUNDITY_H
#define RVF_AEDES_FECUNDITY_H

#include <usbase/model.h>

namespace rvf {

class AedesFecundity : public UniSim::Model
{
public:
    AedesFecundity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double fecundityPerBloodmeal, sexRatio, density, bloodmeal;

    // Variables
    double value;
};

} //namespace


#endif
