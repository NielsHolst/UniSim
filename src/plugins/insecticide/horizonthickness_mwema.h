/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_HORIZONTHICKNESS_H
#define INSECTICIDE_HORIZONTHICKNESS_H

#include <usbase/model.h>

namespace insecticide {

class horizonthickness : public UniSim::Model
{
public:
    horizonthickness(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double startLayer6, startLayer3, startLayer2, startLayer10,
    startLayer11, startLayer8, startLayer7, startLayer9, startLayer5,
    penetrationDepth, startLayer1, startLayer4;

    // Output
    double Htc3, Htc2, Htc4, Htc1, Htc5, Htc6, Htc7, Htc8, Htc9, Htc10;
};

} //namespace


#endif
