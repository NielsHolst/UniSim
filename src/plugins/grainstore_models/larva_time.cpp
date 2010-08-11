/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "larva_time.h"

namespace UniSim{

LarvaTime::LarvaTime(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable<double>("step", &step, this, "desc");
    new PullVariable<double>("total", &total, this, "desc");
}

void LarvaTime::initialize()
{
    setParameter("exponent", &exponent, 16.42, "desc");
    setParameter("minimum", &minimum, 0.001, "desc");
    setParameter("optimum", &optimum, 15.14, "desc");
    setParameter("spread", &spread, 5.169, "desc");
    weather = seekOne<Model*>("weather");
    lactin = seekOneChild<Model*>("time");
}

void LarvaTime::reset() {
    step = total = 0.;
}

void LarvaTime::update()
{
    double gmc = weather->pullVariable<double>("gmc");
    step = lactin->pullVariable<double>("step");
    double scaling = exp(-0.5*pow0(fabs(gmc - optimum)/spread, exponent));
    step *= scaling;
    if (step < minimum)
        step = minimum;
    total += step;
}

} //namespace

