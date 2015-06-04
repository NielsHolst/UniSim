/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_ANNUALAVERAGESOILTEMPERATUREFACTOR_H
#define INSECTICIDE_ANNUALAVERAGESOILTEMPERATUREFACTOR_H

#include <usbase/model.h>

namespace insecticide {

class annualaveragesoiltemperaturefactor : public UniSim::Model
{
public:
    annualaveragesoiltemperaturefactor(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Q, Tref, Tsann;

    // Output
    double FssdT;

};

} //namespace


#endif
