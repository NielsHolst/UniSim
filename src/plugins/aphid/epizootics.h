#ifndef EPIZOOTICS_H
#define EPIZOOTICS_H

#include <usbase/model.h>

namespace aphid {


class epizootics: public UniSim::Model
{
public:
    epizootics(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // parameters
    double insect_number_S, insect_number_I ,ratio_infected_thr;

    // Variables
    double efficiency;

};

#endif // EPIZOOTICS_H

}
