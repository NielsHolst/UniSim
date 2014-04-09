/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MUSSEL_BED_HYDRODYNAMIC_SRSCALE_H
#define MUSSEL_BED_HYDRODYNAMIC__SRSCALE_H

#include <usbase/model.h>

namespace mussel_bed {

class HydrodynamicSRScale : public UniSim::Model
{
public:
    HydrodynamicSRScale(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double maxflow;

    // Variables
    double value;
};

} //namespace


#endif
