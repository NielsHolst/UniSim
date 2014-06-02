/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_GROWTH_H
#define XPEST_GROWTH_H

#include <usbase/model.h>

namespace xpest {

class Damage;

class Growth : public UniSim::Model
{
public:
    Growth(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Parameters
    double k, rue, lai, radiation;

    // Variables
    double laiReduction, rueReduction, production, loss, gain;


    // Links
    QList<Damage*>
        assimilateDemands,
        assimilateDiversions,
        laiReductions,
        rueReductions;

    // Methods
    void setLaiReduction();
    void setRueReduction();
    double assimilateReduction(double production);
};

} //namespace


#endif
