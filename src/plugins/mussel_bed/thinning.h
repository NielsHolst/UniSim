/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef THINNING_H
#define THINNING_H

#include <usbase/model.h>

namespace mussel_bed {

class thinning : public UniSim::Model
{
public:
    thinning(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double Density, N, supply;

    // Variables
    double LossN, LossB, thin, avgs, maxN;
};

} //namespace


#endif
