/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "plant.h"

using namespace UniSim;


namespace conductance {

Plant::Plant(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("initWeight", &initWeight, 2., this,
                 "Initial plant weight at time zero (g per plant)");
    new Parameter<int>("initDay", &initDay, 0, this,
                 "Day of month when growth begins, and @F weight is set to @F {initWeight}. "
                 "Use together with @F {initMonth}. If @F initDay or @F initMonth is zero,"
                 "they are not used and growth will start on day 1 in the simulation");
    new Parameter<int>("initMonth", &initMonth, 0, this,
                 "See @F {initday}");
    new Parameter<double>("A", &A, 0.03, this,
                 "Coefficient in allometric relation for crown zone area: @Math {A w sup phione}");
    new Parameter<double>("phi", &phi, 0.67, this,
                 "Exponent in allometric relation for crown zone area: @Math {A w sup phione}");
    new Parameter<double>("F", &F, 0.01, this,
                 "Coefficient in allometric relation for plant leaf area: @Math {F w sup theta}");
    new Parameter<double>("theta", &theta, 0.9, this,
                 "Exponent in allometric relation for plant leaf area: @Math {F w sup theta}");
    new Parameter<double>("k", &k, 0.6, this,
                 "Light extinction coefficient of foliage [0..1]");
    new Parameter<double>("eps", &eps, 1., this,
                 "Light use efficiency (g\"/\"MJ) of global irradiation");
    new Parameter<double>("n", &n, 20., this,
                 "Plant density (plants per m @Sup {2})");

    new PullVariable<double>("weight", &weight, this,
                     "Plant weight (g per plant");
    new PullVariable<double>("totalWeight", &totalWeight, this,
                     "Total population plant weight (g\"/\"m @Sup 2 ground area available)");
    new PullVariable<double>("sz", &sz, this,
                     "Crown zone area per plant "
                     "(m @Sup 2 ground area owned per per plant)");
    new PullVariable<double>("total_sz", &total_sz, this,
                     "Total population crown zone area "
                     "(m @Sup 2 ground area owned per m @Sup 2 ground area available)");
    new PullVariable<double>("Lz", &Lz, this,
                     "Leaf area index within the crown zone area "
                     "(m @Sup 2 leaf area per m @Sup 2 ground area owned)");
    new PullVariable<double>("fz", &fz, this,
                     "Fraction of light intercepted [0..1]");
    new PullVariable<double>("LA_per_plant", &LA_per_plant, this,
                     "Leaf area per plant (m @Sup 2 leaf area per plant)");
    new PullVariable<double>("dweight", &dweight, this,
                     "Latest increment in plant weight over time step @F dt (g per plant per day)");
    new PullVariable<int>("phase", &_phase, this,
                     "Competition phase: @F Unlimited, "
                     "@F UnderCompression or @F {WeightProportional}.");
    new PullVariable<double>("LAI", &lai, this,
                     "Leaf area index of whole population "
                     "(m @Sup 2 leaf area per m @Sup 2 ground area available");
}

void Plant::initialize() {
    calendar = seekOne<Model*>("calendar");
    weather = seekOne<Model*>("weather");

    other = 0;
    QList<Plant*> siblings = seekSiblings<Plant*>("*");
    if (siblings.size() > 1)
        throw Exception("Max. 2 plants are allowed in community", this);
    other = siblings.isEmpty() ? 0 : siblings[0];
}

void Plant::reset() {
    weight = startAtOnce() ? initWeight : 0.;
    totalWeight = n*initWeight;
    Lz = fz = LA_per_plant = lai = dweight = 0.;
    changePhase(Unlimited);
    updateCrownZoneArea();
}

bool Plant::startAtOnce() const {
    return initDay==0 || initMonth==0;
}

void Plant::changePhase(Phase newPhase) {
    phase = newPhase;
    _phase = int(phase);
    Q_ASSERT(phase == Unlimited || phase == UnderCompression || other);
}

void Plant::update() {
    if (startNow())
        weight = initWeight;
    updateCrownZoneArea();
    updateLightInterception();
    updateWeight();
}

bool Plant::startNow() const {
    QDate today = calendar->pullVariable<QDate>("date");
    return today.day()==initDay && today.month()==initMonth;
}

void Plant::updateCrownZoneArea() {
    switch (phase) {
        case Unlimited:
        sz = (weight==0.) ? 0. : A*pow(weight, phi);
            break;
        case UnderCompression:
            sz = other ? (1. - other->pullVariable<double>("total_sz"))/n : 1./n;
            break;
        case WeightProportional:
            Q_ASSERT(other);
            double totals = totalWeight + other->pullVariable<double>("totalWeight");
            sz = (totals==0.) ? 0. : weight/totals;
    }
    total_sz = n*sz;
    if (total_sz < 0)
        sz = total_sz = 0.;
    else if (total_sz > 1) {
        total_sz = 1.;
        sz = 1./n;
    }
}

void Plant::updateLightInterception() {
    LA_per_plant = (weight==0.) ? 0. : F*pow(weight, theta);
    Lz = (sz==0.) ? 0. : LA_per_plant/sz;
    fz = 1. - exp(-k*Lz);
    lai = n*LA_per_plant;
}

void Plant::updateWeight() {
    double I = weather->pullVariable<double>("irradiation");
    dweight = eps*I*sz*fz;
    weight += dweight;
    totalWeight = n*weight;
}

void Plant::reUpdate() {
    weight -= dweight;
    totalWeight = n*weight;
    update();
}

} //namespace

