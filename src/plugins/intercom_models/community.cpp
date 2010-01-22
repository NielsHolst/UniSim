/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "../standard_models/calendar.h"
#include "community.h"
#include "constants.h"
#include "plant.h"
#include "weather.h"

using namespace UniSim;

namespace intercom{

Community::Community(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Community::initialize()
{
    calendar = findOne<Calendar*>("calendar");
    weather = findOne<Weather*>("weather");
    plants = findChildren<Plant*>("*");
}

void Community::reset()
{
}

void Community::update()
{
    for (int i = 0; i < plants.size(); ++i)
       plants[i]->prepareUpdate();

    updatePlantsByHours();
}

void Community::updatePlantsByHours() {
    double dayLength = calendar->state("dayLength");

    for (int time = 0; time < 3; ++time) {
        s.hour = 12. + 0.5*dayLength*Xgauss2[time];
        s.par = weather->par(s.hour);
        s.sinb = calendar->sinb(s.hour);

        updatePlantsByLayers(time);

        for (int i = 0; i < plants.size(); ++i)
            plants[i]->accumulateLightUseByTime(time);
    }
}

void Community::updatePlantsByLayers(int time) {
    for (int layer = 0; layer < 5; ++layer) {
        s.absorptionExponents.reset();
        for (int i = 0; i < plants.size(); ++i)
            s.absorptionExponents.accumulate( plants[i]->absorptionExponents(layer) );

        for (int i = 0; i < plants.size(); ++i) {
           plants[i]->update();
           plants[i]->accumulateLightUseByLayer(layer);
       }
    }
}

const Community::State* Community::statePtr() const {
    return &s;
}

} //namespace


