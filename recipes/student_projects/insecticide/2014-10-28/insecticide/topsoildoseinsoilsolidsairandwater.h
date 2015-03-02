/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TOPSOILDOSEINSOILSOLIDSAIRANDWATER_H
#define INSECTICIDE_TOPSOILDOSEINSOILSOLIDSAIRANDWATER_H

#include <usbase/model.h>

namespace insecticide {

class topsoildoseinsoilsolidsairandwater : public UniSim::Model
{
public:
    topsoildoseinsoilsolidsairandwater(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Dosers, Za, Zw, Zs, Vs, Vw, Va, VZ;

    // Output
    double Doserss, Dosersw, frsw, Dosersa;
};

} //namespace


#endif
