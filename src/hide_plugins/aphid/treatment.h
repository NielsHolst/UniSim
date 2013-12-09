#ifndef TREATMENT_H
#define TREATMENT_H

#include <usbase/model.h>

namespace aphid {


class treatment : public UniSim::Model
{
public:
    treatment(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // parameters
    double insect_number, damage;

    // Variables
    double control_efficiency;
    bool hasTreated;


};

#endif // TREATMENT_H

}
