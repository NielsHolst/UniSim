/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_FRACTIONOFRAINRUNOFF_H
#define INSECTICIDE_FRACTIONOFRAINRUNOFF_H

#include <usbase/model.h>

namespace insecticide {

class fractionofrainrunoff : public UniSim::Model
{
public:
    fractionofrainrunoff(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Q, P;

    // Output
    double frr;
};

} //namespace


#endif
