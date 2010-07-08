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
    new PullVariable("weight", &weight, this, "Plant weight (g per plant");
    new PullVariable("totalWeight", &totalWeight, this, "Total plant weight (g/m @Sup {2})");
    new PullVariable("sz", &sz, this, "Crown zone area per plant (m @Sup 2 leaf area per per plant)");
    new PullVariable("total_sz", &total_sz, this, "Total crown zone area (m @Sup 2 leaf area per m @Sup 2 ground area)");
    new PullVariable("Lz", &Lz, this, "Leaf area index within the crown zone area (m @Sup 2 leaf area per m @Sup 2 ground area)");
    new PullVariable("fz", &fz, this, "Fraction of light intercepted [0..1]");
    new PullVariable("LA_per_plant", &LA_per_plant, this, "Leaf area per plant (m @Sup 2 leaf area per plant per m @Sup 2 ground area)");
    new PullVariable("dweight", &dweight, this, "Latest increment in plant weight over time step @F dt (g per plant)");
    new PullVariable("phase", &_phase, this, "Growth phase: @F Unlimited (0), @F UnderCompression (1), or @F WeightProportional (2).");
}

void Plant::initialize() {
    setParameter("initWeight", &initWeight, 2., "Initial plant weight at time zero (g per plant)");
    setParameter("A", &A, 0.03, "Coefficient in allometric relation for crown zone area: @Math {A w sup @Sym phi1}");
    setParameter("phi", &phi, 0.67, "Exponent in allometric relation for crown zone area: @Math {A w sup @Sym phi1}");
    setParameter("F", &F, 0.01, "Coefficient in allometric relation for plant leaf area: @Math {F w sup @Sym theta}");
    setParameter("theta", &theta, 0.9, "Exponent in allometric relation for plant leaf area: @Math {F w sup @Sym theta}");
    setParameter("k", &k, 0.6, "Light extinction coefficient of foliage [0..1]");
    setParameter("eps", &eps, 1., "Light use efficiency (g/MJ)");
    setParameter("n", &n, 20., "Plant density (plants per m @Sup {2})");

    weather = seekOne<Model*>("weather");

    other = 0;
    QList<Plant*> siblings = seekSiblings<Plant*>("*");
    if (siblings.size() > 1)
        throw Exception("Max. 2 plants are allowed in community");
    other = siblings.isEmpty() ? 0 : siblings[0];
}

void Plant::reset() {
    weight = initWeight;
    totalWeight = n*initWeight;
    Lz = fz = LA_per_plant = dweight = 0.;
    changePhase(Unlimited);
    updateCrownZoneArea();
}

void Plant::changePhase(Phase newPhase) {
    phase = newPhase;
    _phase = double(phase);
    Q_ASSERT(phase == Unlimited || phase == UnderCompression || other);
}

void Plant::updateByDt(double dt) {
    updateCrownZoneArea();
    updateLightInterception();
    updateWeight(dt);
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
    if (total_sz < 0)
        sz = total_sz = 0.;
    else if (total_sz > 1) {
        total_sz = 1.;
        sz = 1./sz;
    }
}

void Plant::updateLightInterception() {
    LA_per_plant = F*pow(weight, theta);
    Lz = LA_per_plant/sz;
    fz = 1. - exp(-k*Lz);
}

void Plant::updateWeight(double dt) {
    double I = weather->pullVariable("irradiation");
    dweight = eps*I*sz*fz*dt;
    weight += dweight;
    totalWeight = n*weight;
}

} //namespace

