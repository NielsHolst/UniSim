/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_POREWATERVELOCITY_H
#define INSECTICIDE_POREWATERVELOCITY_H

#include <usbase/model.h>

namespace insecticide {

class porewatervelocity : public UniSim::Model
{
public:
    porewatervelocity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double fw, filtervelocity;

    // Output
    double porewatervel;
};

} //namespace


#endif
