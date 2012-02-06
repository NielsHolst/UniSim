/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "fecundity.h"

using namespace UniSim;


namespace beehive{

Fecundity::Fecundity(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("root1", &root1, -10., this, "For parabolic equation");
    new Parameter<double>("root2", &root2, 28., this, "For parabolic equation");
    new Parameter<double>("scale", &scale, 0.02, this, "For parabolic equation");
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Sex ratio (female fraction) of adult population");
    new PullVariable<double>("eggsLaid", &eggsLaid, this, "description");
}

void Fecundity::initialize() {
    adult = seekOneAscendant<Model*>("adult");
}

void Fecundity::reset() {
    eggsLaid = 0;
}

void Fecundity::update() {
    const double *adultAgeClasses = adult->pullVariablePtr<double>("ageClasses");
    int k = adult->parameter<int>("k");
    double duration = adult->parameter<double>("duration");

    eggsLaid = 0;
    for (int i = 0; i < k; ++i) {
        double age = (i + 0.5)*duration/k;
        double numFemales = sexRatio*adultAgeClasses[i];
		eggsLaid +=  numFemales*fecundity(age);
    }
}

double Fecundity::fecundity(double age) const {
    double fec = -scale*(age - root1)*(age - root2);
    return fec < 0. ? 0. : fec;
}
} //namespace

