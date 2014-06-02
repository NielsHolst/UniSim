/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_DEVELOPMENT_H
#define XPEST_DEVELOPMENT_H

#include <usbase/model.h>

namespace xpest {

class Stage;

class DayDegreeScale : public UniSim::Model
{
public:
    DayDegreeScale(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double temperature, cropGrowthStage,
        from, to, T0, duration;

    // Variables
    double completion, growthStage;
    bool completed;

    // Data
    double age;

    // Methods
    void updateFromAge();
};

} //namespace


#endif
