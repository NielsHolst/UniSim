/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STRAWBERRY_PRODUCTION_H
#define STRAWBERRY_PRODUCTION_H

#include <usbase/model.h>

namespace strawberry {

class LeafProduction : public UniSim::Model
{
public:
    LeafProduction(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double productionRate;

    // Variables
    double value;
};

} //namespace


#endif
