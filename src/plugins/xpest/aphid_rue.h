/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_APHID_RUE_H
#define XPEST_APHID_RUE_H

#include "damage.h"

namespace xpest {

class AphidRue : public Damage
{
public:
    AphidRue(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double assimilateEaten,
        honeyProportion, dewProportion, maxRueReduction;

    // Variables
    double honey;
};

} //namespace


#endif
