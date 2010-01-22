/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cfloat>
#include <usbase/utilities.h>
#include "../standard_models/calendar.h"
#include "weather.h"

using namespace std;
using namespace UniSim;

namespace intercom{

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : WeatherFile(name, parent)
{
    setColumn("Tmax", 3);
    setColumn("Tmin", 4);
    setColumn("irradiationMJ", 7);

    setState("Tavg", &Tavg);
    setState("Tday", &Tday);
    setState("irradiation", &irradiation);
    setState("parTotal12h", &par12h.total);
    setState("parDiffuse12h", &par12h.diffuse);
    setState("parDirect12h", &par12h.direct);
}

void Weather::initialize() {
    WeatherFile::initialize();
    calendar = findOne<Calendar*>("calendar");
}

void Weather::update()
{
    WeatherFile::update();
    Tavg = (state("Tmin") + state("Tmax"))/2.;
    Tday = state("Tmax") - 0.25*(state("Tmax") - state("Tmin"));
    irradiation = state("irradiationMJ")*1e6;
    par12h = par(12.);
}

Weather::Par Weather::par(double hour) {
    double
        sinld = calendar->state("sinLD"),
        cosld = calendar->state("cosLD"),
        day = calendar->state("dayInYear"),
        dayLength = calendar->state("dayLength");

    double aob = sinld/cosld;
    double dsinb = 3600.*(dayLength*sinld + 24.*cosld*sqrt(1. - aob*aob)/PI);
    double dsinbe = 3600.*(dayLength*(sinld + 0.4*(sinld*sinld + cosld*cosld*0.5)) +
                           12.*cosld*(2. + 3.*0.4*sinld)*sqrt(1. - aob*aob)/PI);
    double sc = 1370.*(1. + 0.033*cos(2.*PI*day/365.));
    double angot = sc*dsinb;
    double atmtr = irradiation/angot;

    double frdiff;
    if (atmtr >= 0.75)
        frdiff = 0.23;
    else if ((atmtr < 0.75) && (atmtr >= 0.35))
        frdiff = 1.33 - 1.46*atmtr;
    else if ((atmtr < 0.35) && (atmtr >= 0.07))
        frdiff = 1. - 2.3*pow((atmtr - 0.7),2);
    else
        frdiff = 1.;

    double sinbh = calendar->sinb(hour);

    Par par;
    par.total = 0.5*irradiation*sinbh*(1. + 0.4*sinbh)/dsinbe;
    par.diffuse = sinbh*frdiff*atmtr*0.5*sc;
    if (par.diffuse > par.total)
        par.diffuse = par.total;
    par.direct = par.total - par.diffuse;
    return par;
}

} //namespace

