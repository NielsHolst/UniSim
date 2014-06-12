/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_WINDDRIFTTOT_H
#define INSECTICIDE_WINDDRIFTTOT_H

#include <usbase/model.h>
#include "WindDrift.h"

namespace insecticide {

class WindDriftTOT : public WindDrift
{
public:
    WindDriftTOT(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Output
    double totaldrift;
};

} //namespace


#endif
