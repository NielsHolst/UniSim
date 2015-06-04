#ifndef DAMAGETHRESHOLD_H
#define DAMAGETHRESHOLD_H

#include <usbase/model.h>

namespace aphid {



class DamageThreshold : public UniSim::Model
{
public:
    DamageThreshold(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // parameters
    double dayDegreeTimeStep, vegetativePlants, pre_floweringPlants, floweringPlants, maturePlants,
    threshold_vegetativePlants, threshold_pre_floweringPlants, threshold_floweringPlants,
    threshold_maturePlants;

    // Variables
    double threshold;

};

#endif // DAMAGETHRESHOLD_H

}
