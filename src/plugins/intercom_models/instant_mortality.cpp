/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "../unisim_models/stage.h"
#include "instant_mortality.h"
#include "organ.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

InstantMortality::InstantMortality(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<int>("day", &day, 0, this, "Day of month when survival will be applied");
    new Parameter<int>("month", &month, 0, this, "Month when survival will be applied");
    new Parameter<QString>("survival", &survivalAsString, QString(), this,
    "List of survival rates, max. one for each @F Stage defined for the @F {Plant}.");
}

void InstantMortality::initialize() {
    calendar = seekOne<Model*>("calendar");
    Model *plant = seekOneAscendant<Plant*>("*");

    QMap<QString,double> survival = decodeList<QString,double>(survivalAsString);

    QMapIterator<QString, double> su(survival);
    while (su.hasNext()) {
        su.next();
        QString name = su.key();
        double mortality = 100*(1. - su.value());
        TestNum::assureGeZero(mortality, "mortality", this);
        TestNum::assureLe(mortality, 100., "mortality", this);

        Organ *organ = plant->seekOneDescendant<Organ*>(name);
        Stage *mass = organ->seekOneDescendant<Stage*>("mass");
        Stage *area = organ->peekOneDescendant<Stage*>("area");
        victims.append(Victim(mass, mortality));
        if (area)
            victims.append(Victim(area, mortality));
    }
}


void InstantMortality::update() {
    if (calendar->pullValue<int>("day") == day &&
        calendar->pullValue<int>("month") == month) {
        for (int i = 0; i < victims.size(); ++i)
            victims[i].push();
    }
}

InstantMortality::Victim::Victim(UniSim::Model *model_, double mortality_)
    : model(model_), mortality(mortality_)
{
}

void InstantMortality::Victim::push() {
    model->pushValue<double>("instantMortality", mortality);
}


} //namespace

