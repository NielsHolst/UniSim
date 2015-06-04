#include <stdlib.h>

#include "damagethreshold.h"

using namespace std;
using namespace UniSim;

namespace aphid{

DamageThreshold::DamageThreshold(Identifier name, QObject *parent)
    :Model(name, parent)
{
    Input(double, vegetativePlants, 0.); // Proportion of plants in vegetative stage [0;1]
    Input(double, pre_floweringPlants, 0.); // Proportion of plants in flowering stage [0;1]
    Input(double, floweringPlants, 0.); // Proportion of plants in flowering stage [0;1]
    Input(double, maturePlants, 0.); // Proportion of plants in mature stage [0;1]
    Input(double, threshold_vegetativePlants, 5.); // Threshold for aphid in plants in vegetative stage [0;1]
    Input(double, threshold_pre_floweringPlants, 10.); // Threshold for aphid in  plants in flowering stage [0;1]
    Input(double, threshold_floweringPlants, 15.); // Threshold for aphid in  plants in flowering stage [0;1]
    Input(double, threshold_maturePlants, 0.); // Threshold for aphid in  plants in mature stage [0;1]
    Input(double, dayDegreeTimeStep, 0.); // Time step duration (day-degrees)

    Output(double, threshold); // skadeterskel i bladlus pr plante


}
void DamageThreshold::reset(){
    threshold = threshold_vegetativePlants;
}

void DamageThreshold::update(){
    threshold = threshold_vegetativePlants*vegetativePlants+threshold_pre_floweringPlants*pre_floweringPlants+threshold_floweringPlants*floweringPlants;
}

}
