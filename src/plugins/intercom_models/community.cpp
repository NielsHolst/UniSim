/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include <usbase/test_num.h>
#include "area.h"
#include "plant.h"
#include "community.h"

using namespace UniSim;

namespace intercom{

Community::Community(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("earlyGrowthThreshold", &earlyGrowthThreshold, 1., this,
                          "Threshold (LAI) for applying the early exponential growth model");

    new PullVariable<double>("LAI", &lai, this,
                             "Leaf area index of whole community (m @Sup 2 plant area per m @Sup 2 ground)");
    new PullVariable<double>("lightAbsorption", &lightAbsorption, this,
                             "Absorbed light (W per m @Sup 2 ground per day)");
    new PullVariable<double>("CO2Assimilation", &CO2Assimilation, this,
                             "Assimilated kg CO @Sub 2 per ha ground per day");
    new PullVariable<double>("grossProduction", &grossProduction, this,
                             "Produced kg CH @Sub {2}O per ha ground per day");
}

void Community::initialize() {
    areas = seekDescendants<Area*>("*");
    plants = seekDescendants<Plant*>("*");
}

void Community::reset() {
    updateLai();
}

void Community::updateLai() {
    lai = 0.;
    for (int i = 0; i < plants.size(); ++i)
        lai += plants[i]->pullVariable<double>("lai");
}

void Community::update() {
    updateLai();
    if (lai < earlyGrowthThreshold) {
        updateEarlyGrowth();
        accumulateLateGrowth();
    }
    else {
        accumulateLateGrowth();
        updateLateGrowth();
    }
}

void Community::updateEarlyGrowth() {
    for (int i = 0; i < plants.size(); ++i)
        plants[i]->applyEarlyGrowth();
}


void Community::accumulateLateGrowth() {
    // Update area photosynthesis
    for (int hourPoint = 0; hourPoint < 3; ++hourPoint) {
        for (int heightPoint = 0; heightPoint < 5; ++heightPoint) {
            for (int i = 0; i < areas.size(); ++i) {
                areas[i]->setPoint(hourPoint, heightPoint);
                areas[i]->updatePhotosynthesis(sumELAI());
            }
        }
    }
    // Accumulate community-wide rates
    lightAbsorption = CO2Assimilation = 0.;
    for (int i = 0; i < areas.size(); ++i) {
        lightAbsorption += areas[i]->pullVariable<double>("lightAbsorption");
        CO2Assimilation += areas[i]->pullVariable<double>("CO2Assimilation");
    }

    grossProduction = 0;
    for (int i = 0; i < plants.size(); ++i) {
        plants[i]->accumulate();
        double assimilation = plants[i]->pullVariable<double>("CO2assimilation");
        grossProduction += assimilation*30./44.;
    }

}

void Community::updateLateGrowth() {
    for (int i = 0; i < plants.size(); ++i) {
        double assimilation = plants[i]->pullVariable<double>("CO2assimilation");
        double grossProduction = assimilation*30./44.;
        plants[i]->allocate(grossProduction);
    }
}

const double* Community::sumELAI() {
    _sumELAI[0] = _sumELAI[1] = _sumELAI[2] = 0.;
    for (int i = 0; i < areas.size(); ++i) {
        const double *elai = areas[i]->calcELAI();
        for (int i = 0; i < 3; ++i)
            _sumELAI[i] += elai[i];
    }
    return _sumELAI;
}

} //namespace

