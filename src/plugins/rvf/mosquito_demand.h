/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RVF_MOSQUITO_DEMAND_H
#define RVF_MOSQUITO_DEMAND_H

#include <usbase/model.h>

namespace rvf {

class MosquitoDemand : public UniSim::Model
{
public:
    MosquitoDemand(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double sexRatio, density, demandRate;

    // Variables
    double value;
};

} //namespace


#endif
