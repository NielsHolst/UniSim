/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_SOILDEGRADATIONRATECONSTANT_H
#define INSECTICIDE_SOILDEGRADATIONRATECONSTANT_H

#include <usbase/model.h>

namespace insecticide {

class soildegradationrateconstant : public UniSim::Model
{
public:
    soildegradationrateconstant(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double DT50;

    // Output
    double k;
};

} //namespace


#endif
