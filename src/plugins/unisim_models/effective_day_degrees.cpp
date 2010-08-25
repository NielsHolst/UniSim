/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "photo_thermal_time.h"

namespace UniSim{

EffectiveDayDegrees::EffectiveDayDegrees(UniSim::Identifier name, QObject *parent)
    : DayDegrees(name, parent)
{
}

void EffectiveDayDegrees::initialize()
{
    DayDegrees::initialize();
	new Parameter<double>("q", &q, 0.1);
    weather = findOne<Model*>("weather");
}

void EffectiveDayDegrees::update()
{
    double prevTotal = total;
    DayDegrees::update();
	double par = weather->state("irradiation")/2.;
    step = 1./( 1./step + q/par );
    total = prevTotal + step;
}

} //namespace

