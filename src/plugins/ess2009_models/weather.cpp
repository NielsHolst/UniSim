/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <cfloat>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "weather.h"

using namespace std;
using namespace UniSim;

namespace ess2009 {

Weather::Weather(UniSim::Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    new PullVariable("T", &_T, this, "description");
    new PullVariable("Tavg", &_T, this, "description");
    new PullVariable("Tsum", &_Tsum, this, "description");
}

void Weather::initialize()
{
    setParameter("tempOffset", &_tempOffset, -0.75, "description");
    setParameter("tempB", &_tempB, 17.20, "description");
    setParameter("tempC", &_tempC, 2.901, "description");
    setParameter("tempD", &_tempD, 722.7, "description");
    _calendar = seekOne<Model*>("calendar");
}	

void Weather::reset()
{
    _Tsum = 0;
}

void Weather::update()
{
    int dayOfYear = int(_calendar->pullVariable("dayOfYear"));
    _T = temperature(dayOfYear);
    _Tsum = (dayOfYear == 1) ? _T : _Tsum + _T;
}

double Weather::temperature(int dayOfYear) const {
    double k = sin(2*PI*dayOfYear/_tempD + _tempC);
    return _tempOffset + _tempB*k*k;
}

}
