/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIPAPHIS_APHID_FECUNDITY_H
#define LIPAPHIS_APHID_FECUNDITY_H

#include <usbase/model.h>

namespace lipaphis {

class AphidFecundity : public UniSim::Model
{
public:
    AphidFecundity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    double nitrogen, intercept, slope, lifeTimeFecundity;
};

} //namespace


#endif
