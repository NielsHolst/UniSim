/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_PEST_H
#define XPEST_PEST_H

#include <usbase/model.h>

namespace xpest {

class Damage : public UniSim::Model
{
public:
    Damage(UniSim::Identifier name, QObject *parent);
    void reset();

protected:
    // Variables
    double damage;
};

} //namespace


#endif
