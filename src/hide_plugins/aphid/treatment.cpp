#include <stdlib.h>

#include "treatment.h"

using namespace std;
using namespace UniSim;

namespace aphid{

treatment::treatment(Identifier name, QObject *parent)
    :Model(name, parent)
{
    addParameter<double>(Name(insect_number), 0., "Proportion of plants in vegetative stage [0;1]");
    addParameter<double>(Name(damage), 0., "Proportion of plants in flowering stage [0;1]");

    addVariable<double>(Name(control_efficiency), "skadeterskel i bladlus pr plante");


}
void treatment::reset(){
    control_efficiency = 0;
    hasTreated = false;
}

void treatment::update(){
    control_efficiency = 0;
    if (insect_number > damage && !hasTreated) {
        control_efficiency = 95;
        hasTreated = true;
    }
}

}

