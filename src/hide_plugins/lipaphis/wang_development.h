/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIPAPHIS_WANG_DEVELOPMENT_H
#define LIPAPHIS_WANG_DEVELOPMENT_H

#include <usbase/model.h>

namespace lipaphis {

class WangDevelopment : public UniSim::Model
{
public:
    WangDevelopment(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    double T,
        Tlow, Topt, Tmax, delta, r, H,
        step;
};

} //namespace


#endif
