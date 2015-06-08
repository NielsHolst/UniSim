/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef RVF_MOSQUITOINFECTION_H
#define RVF_MOSQUITOINFECTION_H

#include <usbase/model.h>

namespace rvf {

class MosquitoInfection : public UniSim::Model
{
public:
    MosquitoInfection(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double infectionRate, bloodmeal, duration, transovarial,density;

    // Variables
    double value;

    //Links

};

} //namespace

#endif // RVF_MOSQUITODEMAND_H

