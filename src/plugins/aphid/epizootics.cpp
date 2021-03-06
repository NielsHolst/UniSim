#include <stdlib.h>
#include "epizootics.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace aphid{

PUBLISH(epizootics)

epizootics::epizootics(Identifier name, QObject *parent)
    :Model(name, parent)
{
    Input(double, insect_number_S, 0.); // Proportion of plants in vegetative stage [0;1]
    Input(double, insect_number_I, 0.); // Proportion of plants in vegetative stage [0;1]
    Input(double, ratio_infected_thr, 0.); // Proportion of plants in flowering stage [0;1]

    Output(double, efficiency); // skadeterskel i bladlus pr plante


}
void epizootics::reset(){
    efficiency = 0;
}

void epizootics::update(){
    efficiency = 0;
    if (insect_number_I/(insect_number_S+insect_number_I) > ratio_infected_thr)
        efficiency = 50;
}

}
