/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PRODUSPLOTS_H
#define PRODUSPLOTS_H

#include <usbase/model.h>

namespace mussel_bed {

class Produsplots : public UniSim::Model
{
public:
    Produsplots(UniSim::Identifier name, QObject *parent);


private:
    // Parameters
    double stDensity, mDensity, days;
    QDate startDate;

};

} //namespace


#endif
