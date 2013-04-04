/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RVF_WATER_LEVEL_H
#define RVF_WATER_LEVEL_H

#include <usbase/model.h>

namespace rvf {

class WaterLevel : public UniSim::Model
{
public:
    WaterLevel(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double initialValue, dailyRainfall, dailyLoss;

    // Variables
    double value;
};

} //namespace


#endif
