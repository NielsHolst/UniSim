/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_SOILPOROSITY_H
#define INSECTICIDE_SOILPOROSITY_H

#include <usbase/model.h>

namespace insecticide {

class soilporosity : public UniSim::Model
{
public:
    soilporosity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double fa, fw;

    // Output
    double Sp;
};

} //namespace


#endif
