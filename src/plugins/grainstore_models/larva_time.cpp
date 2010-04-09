/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "larva_time.h"

namespace UniSim{

LarvaTime::LarvaTime(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("step", &step);
    setState("total", &total);
}

void LarvaTime::initialize()
{
    setParameter("exponent", &exponent, 16.42);
    setParameter("minimum", &minimum, 0.001);
    setParameter("optimum", &optimum, 15.14);
    setParameter("spread", &spread, 5.169);
    weather = seekOne<Model*>("weather");
    lactin = seekOneChild<Model*>("time");
}

void LarvaTime::reset() {
    step = total = 0.;
}

void LarvaTime::update()
{
    double gmc = weather->state("gmc");
    step = lactin->state("step");
    double scaling = exp(-0.5*pow0(fabs(gmc - optimum)/spread, exponent));
    step *= scaling;
    if (step < minimum)
        step = minimum;
    total += step;
}

} //namespace

