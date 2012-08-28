/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
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
    new Parameter<bool>("testMode", &testMode, false, this,
    "In test mode, the plants stop growing after @F earlyGrowthThreshold has been reached. "
    "Daily photosynthesis is calculated but not allocated");

    new Variable<bool>("isInEarlyGrowth", &isEarly, this,
                           "Is the community it its early growth phase?");
    new Variable<double>("LAI", &lai, this,
                             "Leaf area index of whole community (m @Sup 2 plant area per m @Sup 2 ground)");
    new Variable<double>("lightAbsorption", &lightAbsorption, this,
                             "Absorbed light (W per m @Sup 2 ground per day)");
    new Variable<double>("CO2Assimilation", &CO2Assimilation, this,
                             "Assimilated kg CO @Sub 2 per ha ground per day");
    new Variable<double>("grossProduction", &grossProduction, this,
                             "Produced kg CH @Sub {2}O per ha ground per day");

}

void Community::initialize() {
    areas = seekDescendants<Area*>("*");
    plants = seekDescendants<Plant*>("*");
}

void Community::reset() {
    isEarly = true;
    updateLai();
}

void Community::updateLai() {
    lai = 0.;
    for (int i = 0; i < plants.size(); ++i)
        lai += plants[i]->pullValue<double>("lai");
}

void Community::update() {
    // Total LAI
    updateLai();
    // Areas
    for (int hourPoint = 0; hourPoint < 3; ++hourPoint) {
        for (int heightPoint = 0; heightPoint < 5; ++heightPoint) {
            updateSumELAI();
            for (int i = 0; i < areas.size(); ++i) {
                areas[i]->setPoint(hourPoint, heightPoint);
                areas[i]->updatePhotosynthesis(sumELAI);
            }
        }
    }
    // Plants
    lightAbsorption =
    CO2Assimilation =
    grossProduction =
    maintenanceResp = 0.;
    for (int i = 0; i < plants.size(); ++i) {
        Plant *plant = plants[i];
        if (lai < earlyGrowthThreshold && isEarly) {
            plant->updateEarlyGrowth();
        }
        else {
            isEarly = false;
            plant->updatePhotosynthesis();
        }
        lightAbsorption += plant->pullValue<double>("lightAbsorption");
        CO2Assimilation += plant->pullValue<double>("CO2Assimilation");
        grossProduction += plant->pullValue<double>("grossProduction");
        maintenanceResp += plant->pullValue<double>("maintenanceResp");
        growthResp += plant->pullValue<double>("growthResp");
        netAllocation += plant->pullValue<double>("netAllocation");
    }
}

void Community::updateSumELAI() {
    sumELAI[0] = sumELAI[1] = sumELAI[2] = 0.;
    for (int i = 0; i < areas.size(); ++i) {
        const double *elai = areas[i]->calcELAI();
        for (int i = 0; i < 3; ++i)
            sumELAI[i] += elai[i];
    }
}

} //namespace

