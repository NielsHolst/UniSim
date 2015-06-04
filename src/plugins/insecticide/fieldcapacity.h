/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_FIELDCAPACITY_H
#define INSECTICIDE_FIELDCAPACITY_H

#include <usbase/model.h>

namespace insecticide {

class fieldcapacity : public UniSim::Model
{
public:
    fieldcapacity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double fsandLayer6, fsandLayer3, fsandLayer2, fsandLayer10, fsandLayer1,
    fsandLayer8, fsandLayer7, fsandLayer9, fsandLayer5, fsandLayer4,
    Htc3, Htc2, Htc4, Htc1, Htc5, Htc6, Htc7, Htc8, Htc9, Htc10;

    // Output
    double fieldcap;
};

} //namespace


#endif
