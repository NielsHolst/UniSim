/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_FRACTIONDRAINAGEDEPTH_H
#define INSECTICIDE_FRACTIONDRAINAGEDEPTH_H

#include <usbase/model.h>

namespace insecticide {

class fractiondrainagedepth : public UniSim::Model
{
public:
    fractiondrainagedepth(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double startLayer6, startLayer3, startLayer2, startLayer10,
    startLayer11, startLayer8, startLayer7, startLayer9, startLayer5,
    Dds, startLayer1, startLayer4, Dp1, Dp2, Dp3, Dp4,
    Dp5, Dp6, Dp7, Dp8, Dp9, Dp10, Htc3, Htc2, Htc4, Htc1, Htc5,
    Htc6, Htc7, Htc8, Htc9, Htc10;

    // Output
    double Fdd3, Fdd2, Fdd4, Fdd1, Fdd5, Fdd6, Fdd7, Fdd8, Fdd9, Fdd10;
};

} //namespace


#endif
