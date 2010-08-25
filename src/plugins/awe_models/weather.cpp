/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <cfloat>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "weather.h"

using namespace std;
using namespace UniSim;

namespace awe {

Weather::Weather(UniSim::Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    new Parameter<double>("a", &a, -0.75, this,
                 "Parameter @I a of yearly sine curve for temperature (@Char ring C)");
    new Parameter<double>("b", &b, 17.20, this,
                 "Parameter @I b of yearly sine curve for temperature (@Char ring C)");
    new Parameter<double>("c", &c, 2.901, this,
                 "Parameter @I c of yearly sine curve for temperature (dimensionless)");

    new PullVariable<double>("T", &T, this, "Daily average temperature (@Char ring C)");
    new PullVariable<double>("Tavg", &T, this, "Same as @F {T}");
    new PullVariable<double>("Tsum", &Tsum, this,
                     "Temperature sum since 1 January "
                     "Day-degrees above 0 @Char ring C");
}

void Weather::initialize()
{
    calendar = seekOne<Model*>("calendar");
}	

void Weather::reset()
{
    Tsum = 0;
}

void Weather::update()
{
    int dayOfYear = int(calendar->pullVariable<double>("dayOfYear"));
    T = temperature(dayOfYear);
    Tsum = (dayOfYear == 1) ? T : Tsum + T;
}

double Weather::temperature(int dayOfYear) const {
    double k = sin(PI*dayOfYear/365. + c);
    return a + b*k*k;
}

}
