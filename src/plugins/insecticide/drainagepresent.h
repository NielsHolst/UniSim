/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DRAINAGEPRESENT_H
#define INSECTICIDE_DRAINAGEPRESENT_H

#include <usbase/model.h>

namespace insecticide {

class drainagepresent : public UniSim::Model
{
public:
    drainagepresent(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double startLayer6, startLayer3, startLayer2, startLayer10,
    startLayer11, startLayer8, startLayer7, startLayer9, startLayer5,
    Dds, startLayer1, startLayer4;

    // Output
    double Dp3, Dp2, Dp4, Dp1, Dp5, Dp6, Dp7, Dp8, Dp9, Dp10;
};

} //namespace


#endif
