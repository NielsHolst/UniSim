/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_REDUCTION_H
#define XPEST_REDUCTION_H

#include "damage.h"

namespace xpest {

class Reduction : public Damage
{
public:
    Reduction(UniSim::Identifier name, QObject *parent);
    void amend();
    void update();
private:
    // Parameters
    double pestSeverity, maxDamage, beta;
    // Links
    QList<Reduction*> children;
};

} //namespace


#endif
