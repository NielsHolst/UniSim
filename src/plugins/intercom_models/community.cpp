/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/debug_output.h>
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
    calendar = seekOne<Calendar*>("calendar");
    weather = seekOne<Weather*>("weather");
    plants = seekChildren<Plant*>("*");
}

void Community::reset()
{
}

void Community::update()
{
    for (int i = 0; i < plants.size(); ++i)
       plants[i]->prepareUpdate();

    updateWeightedAreaAboveLayer();
    updatePlantsByHours();
}

void Community::updateWeightedAreaAboveLayer() {
    for (int layerStep = 0; layerStep < 5; ++layerStep) {
        s.weightedAreaAboveLayer[layerStep].reset();
        for (int i = 0; i < plants.size(); ++i) {
            s.weightedAreaAboveLayer[layerStep].accumulate(
                    plants[i]->weightedAreaAboveLayer(layerStep) );
        }
    }
}

void Community::updatePlantsByHours() {
    double dayLength = calendar->state("dayLength");

    for (s.timeStep = 0; s.timeStep < 3; ++s.timeStep) {
        s.hour = 12. + 0.5*dayLength*Xgauss2[s.timeStep];
        s.par = weather->par(s.hour);
        s.sinb = calendar->sinb(s.hour);

        updatePlantsByLayers();

        for (int i = 0; i < plants.size(); ++i)
            plants[i]->accumulateLightUseByTime(s.timeStep);
    }
}

void Community::updatePlantsByLayers() {
    for (s.layerStep = 0; s.layerStep < 5; ++s.layerStep) {
        for (int i = 0; i < plants.size(); ++i) {
           plants[i]->update();
           plants[i]->accumulateLightUseByLayer(s.layerStep);
        }
    }
}

const Community::State* Community::statePtr() const {
    return &s;
}

} //namespace


