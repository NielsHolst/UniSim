/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "plant.h"

using namespace UniSim;


namespace conductance {

Plant::Plant(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new PullVariable("weight", &weight, this, "description");
    new PullVariable("totalWeight", &totalWeight, this, "description");
    new PullVariable("sz", &sz, this, "description");
    new PullVariable("total_sz", &total_sz, this, "description");
    new PullVariable("Lz", &Lz, this, "description");
    new PullVariable("fz", &fz, this, "description");
    new PullVariable("LA_per_plant", &LA_per_plant, this, "description");
    new PullVariable("dweight", &dweight, this, "description");
    new PullVariable("phase", &_phase, this, "description");
}

void Plant::initialize() {
    setParameter("initWeight", &initWeight, 2., "description");
    setParameter("A", &A, 0.03, "description");
    setParameter("phi", &phi, 0.67, "description");
    setParameter("F", &F, 0.01, "description");
    setParameter("theta", &theta, 0.9, "description");
    setParameter("k", &k, 0.6, "description");
    setParameter("eps", &eps, 1., "description");
    setParameter("n", &n, 20., "description");

    weather = seekOne<Model*>("weather");

    other = 0;
    QList<Plant*> siblings = seekSiblings<Plant*>("*");
    if (siblings.size() > 1)
        throw Exception("Max. 2 plants are allowed in community");
    other = siblings.isEmpty() ? 0 : siblings[0];
}

void Plant::reset() {
    weight = initWeight;
    changePhase(Unlimited);
    updateCrownZoneArea();
}

void Plant::updateByDt(double dt) {
    updateCrownZoneArea();

    LA_per_plant = F*pow(weight, theta);
    Lz = LA_per_plant/sz;
    fz = 1. - exp(-k*Lz);

    double I = weather->pullVariable("irradiation");
    dweight = eps*I*sz*fz*dt;
    weight += dweight;
    totalWeight = n*weight;
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
            sz = other ? (1. - other->pullVariable("total_sz"))/n : 1./n;
            break;
        case WeightProportional:
            Q_ASSERT(other);
            sz = weight/(totalWeight + other->pullVariable("totalWeight"));
    }
    total_sz = n*sz;
    if (total_sz < 0) {
        if (total_sz > 1e-6)
            total_sz = 0.;
        else {
            QString msg = "Decrease time step. Total crown zone area is negative; total_sz = " +
                          QString::number(total_sz);
            throw Exception(msg);
        }
    }
    if (total_sz > 1) {
        if (total_sz < 1. + 1e-6)
            total_sz = 1.;
        else {
            QString msg = "Decrease time step. Total crown zone area > 1 m2; total_sz = " +
                          QString::number(total_sz);
            throw Exception(msg);
        }
    }
}

} //namespace

