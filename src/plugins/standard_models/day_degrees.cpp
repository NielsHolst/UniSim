/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "day_degrees.h"

namespace UniSim{

DayDegrees::DayDegrees(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable("step", &step, this);
    new PullVariable("total", &total, this);
}

void DayDegrees::initialize()
{
    setParameter("T0", &T0, 0.);
    setParameter("Topt", &Topt, 100.);
    setParameter("Tmax", &Tmax, 100.);
    weather = seekOne<Model*>("weather");
}

void DayDegrees::reset() {
    step = total = 0.;
}

void DayDegrees::update()
{
    double T = weather->pullVariable("Tavg");
    if (T < T0)
        step = 0.;
    else if (T < Topt)
        step = T - T0;
    else if (T < Tmax)
        step = Tmax - T;
    else
        step = 0;
    total += step;
}

} //namespace

