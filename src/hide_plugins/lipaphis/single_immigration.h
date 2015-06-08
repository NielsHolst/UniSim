/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIPAPHIS_SINGLE_IMMIGRATION_H
#define LIPAPHIS_SINGLE_IMMIGRATION_H

#include <usbase/model.h>

namespace lipaphis {

class SingleImmigration : public UniSim::Model
{
public:
    SingleImmigration(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    int immigrationDay, currentDay;
    double Anumber, Pnumber, Fimmigrants, Mimmigrants, immigrants;
};

} //namespace


#endif
