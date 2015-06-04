/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_RETARDATIONTIME_H
#define INSECTICIDE_RETARDATIONTIME_H

#include <usbase/model.h>

namespace insecticide {

class retardationtime : public UniSim::Model
{
public:
    retardationtime(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Rf1, Rf2, Rf3, Rf4, Rf5,
    Rf6, Rf7, Rf8, Rf9, Rf10,
    Irt1, Irt2, Irt3, Irt4, Irt5, Irt6, Irt7, Irt8, Irt9, Irt10;

    // Output
    double Rt1, Rt2,Rt3,
    Rt4, Rt5, Rt6,
    Rt7, Rt8, Rt9,
    Rt10;
};

} //namespace


#endif
