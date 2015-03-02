/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_HTC_H
#define INSECTICIDE_HTC_H

#include <usbase/model.h>

namespace insecticide {

class htc : public UniSim::Model
{
public:
    htc(UniSim::Identifier name, QObject *parent);    
    void reset();
    void update();

private:
    // Input
    double penetrationDepth;

    // Output
    double Htc3, Htc2, Htc4, Htc1, Htc5, Htc6, Htc7, Htc8, Htc9, Htc10;
};

} //namespace

#endif
