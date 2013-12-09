/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_SUBSTANCE_H
#define INSECTICIDE_SUBSTANCE_H

#include <usbase/model.h>

namespace insecticide {

class Substance : public UniSim::Model
{
public:
    Substance(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double inflow, degradationConstant;

    // Variables
    double concentration, outflow;
};

} //namespace


#endif
