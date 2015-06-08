/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "plant.h"
#include "publish.h"

using namespace UniSim;


namespace conductance {

PUBLISH(Plant)

Plant::Plant(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, initWeight, 2.); /*
                 "Initial plant weight at time zero (g per plant) */
    Input(int, initDay, 0); /*
                 "Day of month when growth begins and @F weight is set to @F {initWeight}. "
                 "Use together with @F {initMonth}. If @F initDay or @F initMonth is zero,"
                 "they are not used and growth will start on day 1 in the simulation */
    Input(int, initMonth, 0); /*
                 "See @F {initDay} */
    Input(double, A, 0.03); /*
                 "Coefficient in allometric relation for crown zone area: @Math {A w sup phione} */
    Input(double, phi, 0.67); /*
                 "Exponent in allometric relation for crown zone area: @Math {A w sup phione} */
    Input(double, F, 0.01); /*
                 "Coefficient in allometric relation for plant leaf area: @Math {F w sup theta} */
    Input(double, theta, 0.9); /*
                 "Exponent in allometric relation for plant leaf area: @Math {F w sup theta} */
    Input(double, k, 0.6); /*
                 "Light extinction coefficient of foliage [0..1] */
    Input(double, eps, 1.); /*
                 "Light use efficiency (g/MJ) of global irradiation */
    Input(double, n, 20.); /*
                 "Plant density (plants per m @Sup {2}) */

    Output(double, weight); /*
                     "Plant weight (g per plant) */
    Output(double, totalWeight); /*
                     "Total population plant weight (g/m @Sup 2 ground area available) */
    Output(double, sz); /*
                     "Crown zone area per plant "
                     "(m @Sup 2 ground area owned per per plant) */
    Output(double, total_sz); /*
                     "Total population crown zone area "
                     "(m @Sup 2 ground area owned per m @Sup 2 ground area available) */
    Output(double, Lz); /*
                     "Leaf area index within the crown zone area "
                     "(m @Sup 2 leaf area per m @Sup 2 ground area owned) */
    Output(double, fz); /*
                     "Fraction of light intercepted [0..1] */
    Output(double, LA_per_plant); /*
                     "Leaf area per plant (m @Sup 2 leaf area per plant) */
    Output(double, dweight); /*
                     "Latest increment in plant weight over time step @F dt (g per plant per day) */
    new Variable<int>("phase", &_phase, this, ""); /*
    Output2(int, phase, _phase);
                     "Competition phase: @F Unlimited, "
                     "@F UnderCompression or @F {WeightProportional}. */
    Output(double, lai); /*
                     "Leaf area index of whole population "
                     "(m @Sup 2 leaf area per m @Sup 2 ground area available) */
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
    QDate today = calendar->pullValue<QDate>("date");
    return today.day()==initDay && today.month()==initMonth;
}

void Plant::updateCrownZoneArea() {
    switch (phase) {
        case Unlimited:
        sz = (weight==0.) ? 0. : A*pow(weight, phi);
            break;
        case UnderCompression:
            sz = other ? (1. - other->pullValue<double>("total_sz"))/n : 1./n;
            break;
        case WeightProportional:
            Q_ASSERT(other);
            double totals = totalWeight + other->pullValue<double>("totalWeight");
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
    double I = weather->pullValue<double>("irradiation");
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

