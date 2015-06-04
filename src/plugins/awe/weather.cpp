/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <cfloat>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "publish.h"
#include "weather.h"

using namespace std;
using namespace UniSim;

namespace awe {

PUBLISH(Weather)

Weather::Weather(UniSim::Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    Input(double, a, -0.75); /*
                 "Parameter @I a of yearly sine curve for temperature (@Char{ring}C) */
    Input(double, b, 17.20); /*
                 "Parameter @I b of yearly sine curve for temperature (@Char{ring}C)"); */

    Output(double, T); // Daily average temperature (@Char{ring} C)
    Output2(double, T, Tavg); // Same as @F {T}
    Output(double, Tsum); // Temperature sum since 1 January. Day-degrees above 0 @Char{ring}C
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
    int dayOfYear = calendar->pullValue<int>("dayOfYear");
    T = temperature(dayOfYear);
    Tsum = (dayOfYear == 1) ? T : Tsum + T;
}

double Weather::temperature(int dayOfYear) const {
    double k = sin(PI*dayOfYear/365.);
    return a + b*k*k;
}

}
