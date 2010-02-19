/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "plant.h"

using namespace UniSim;


namespace conductance {

Plant::Plant(UniSim::Identifier name, QObject *parent)
    : Model(name, parent) {
    setState("weight", &weight);
    setState("totalWeight", &totalWeight);
    setState("sz", &sz);
    setState("total_sz", &total_sz);
    setState("Lz", &Lz);
    setState("fz", &fz);
    setState("dweight", &dweight);
    setState("phase", &_phase);
}

void Plant::initialize() {
    setParameter("initWeight", &initWeight, 2.);
    setParameter("A", &A, 0.03);
    setParameter("phi", &phi, 0.67);
    setParameter("F", &F, 0.01);
    setParameter("theta", &theta, 0.9);
    setParameter("k", &k, 0.6);
    setParameter("eps", &eps, 1.);
    setParameter("n", &n, 20.);

    weather = findOne<Model*>("weather");

    other = 0;
    QList<Plant*> plants = UniSim::findChildren<Plant*>("*", parent());
    if (plants.size() > 2)
        throw Exception("Max. 2 plants are allowed in community");
    for (int i = 0; i < plants.size(); ++i) {
        if (plants[i] != this)
            other = plants[i];
    }

}

void Plant::reset() {
    weight = initWeight;
    changePhase(Unlimited);
}

void Plant::update() {
    updateCrownZoneArea();

    Lz = F*pow(weight, theta)/sz;
    fz = 1. - exp(-k*Lz);

    double I = weather->state("irradiation");
    dweight = eps*I*sz*fz;
    weight += dweight;
    totalWeight = n*weight;
    total_sz = n*sz;
}

void Plant::changePhase(Phase newPhase) {
    phase = newPhase;
    _phase = double(phase);
    Q_ASSERT(phase == Unlimited || phase == UnderCompression || other);
}

void Plant::updateCrownZoneArea() {
    switch (phase) {
        case Unlimited:
            sz = A*pow(weight, phi);
            break;
        case UnderCompression:
            sz = (1. - other->state("total_sz"))/n;
            break;
        case WeightProportional:
            sz = weight/(totalWeight + other->state("totalWeight"));
    }
}

} //namespace

