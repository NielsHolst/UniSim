/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include <usbase/utilities.h>
#include "../unisim_models/calendar.h"
#include "mass.h"

using namespace UniSim;

namespace intercom{

Mass::Mass(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    // parameters
    double initialMass, initialPctN, respRate, epsMass;

    // pull variables
    double mass;

    // push-pull variables
    double pctN;

    // push variables
    double massChange;

    new Parameter<double>("initialMass", &initialMass, 0.01, this,
                          "Mass at time 0 (\"g/m\" @Sup {2})");
    new Parameter<double>("initialPctN", &initialPctN, 5., this,
                          "Percentage nitrogen in @F mass at time 0");
    new Parameter<double>("respRate", &respRate, 0.013, this,
                          "Respiration rate at 20 {@Degree}C \"(g/g/day)\"");
    new Parameter<double>("epsMass", &epsMass, 1e-6, this,
                          "Precision of mass. If mass gets negative but not by more than @F epsMass "
                          "it is set to zero, otherwise an exception is thrown");
    new PullVariable<double>("mass", &mass, this,
                             "Mass density (\"g/m\" @Sup {2})");
    new PullVariable<double>("pctN", &pctN, this,
                             "Percentage nitrogen in @F {mass}");
    new PushVariable<double>("pctN", &pctN, this,
                             "Percentage nitrogen in @F {mass}");
    new PushVariable<double>("massChange", &massChange, this,
                             "Change in mass density (\"g/m\" @Sup {2}\"/day\")");
}

}

void Mass::reset() {
    mass = initialMass;
    pctN = initialPctN;
}

void Mass::update() {
    mass += massChange;
    massChange = 0.;
    if (mass < 0) {
        if (mass > -massEps)
            mass = 0;
        else
            throw Exception("Negative mass = " + QString::number(mass), this);
    }
}


} //namespace

