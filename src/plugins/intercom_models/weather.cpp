/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cfloat>
#include <usbase/clock.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
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

    new PullVariable("Tavg", &Tavg, this, "description");
    new PullVariable("Tday", &Tday, this, "description");
    new PullVariable("irradiation", &irradiation, this, "description");
    new PullVariable("parTotal", &par.total, this, "description");
    new PullVariable("parDiffuse", &par.diffuse, this, "description");
    new PullVariable("parDirect", &par.direct, this, "description");
}

void Weather::initialize() {
    WeatherFile::initialize();
    calendar = seekOne<Calendar*>("calendar");
    verifySequence();
    connect(clock(), SIGNAL(tick(double)), this, SLOT(handleClockTick(double)));
}

void Weather::verifySequence() {
    Simulation *simulation = seekOneAscendant<Simulation*>("*");
    const Models &sequence(simulation->sequence());
    int ixCalendar(-1), ixWeather(-1);
    for (int i = 0; i < sequence.size(); ++i) {
        if (sequence.at(i) == calendar) ixCalendar = i;
        if (sequence.at(i) == this) ixWeather = i;
    }
    Q_ASSERT(ixCalendar > -1 && ixWeather > -1);
    if (ixCalendar > ixWeather)
        throw Exception("Calendar must appear before weather in simulation sequence. Reorder 'sequence' elements in XML file.");
}

void Weather::update()
{
    WeatherFile::update();
    Tavg = (pullVariable("Tmin") + pullVariable("Tmax"))/2.;
    Tday = pullVariable("Tmax") - 0.25*(pullVariable("Tmax") - pullVariable("Tmin"));
    irradiation = pullVariable("irradiationMJ")*1e6;
}

void Weather::handleClockTick(double hour) {
    updatePar();
}

void Weather::updatePar() {
    double
        sinld = calendar->pullVariable("sinLD"),
        cosld = calendar->pullVariable("cosLD"),
        day = calendar->pullVariable("dayInYear"),
        dayLength = calendar->pullVariable("dayLength");

    double aob = sinld/cosld;
    double dsinb = 3600.*(dayLength*sinld + 24.*cosld*sqrt(1. - aob*aob)/PI);
    double dsinbe = 3600.*(dayLength*(sinld + 0.4*(sinld*sinld + cosld*cosld*0.5)) +
                           12.*cosld*(2. + 3.*0.4*sinld)*sqrt(1. - aob*aob)/PI);
    double sc = 1370.*(1. + 0.033*cos(2.*PI*day/365.));
    double angot = sc*dsinb;
    double atmtr = irradiation/angot;

    double frdiff;
    if (atmtr > 0.75)
        frdiff = 0.23;
    else if (atmtr > 0.35)
        frdiff = 1.33 - 1.46*atmtr;
    else if (atmtr > 0.07)
        frdiff = 1. - 2.3*pow((atmtr - 0.07),2);
    else
        frdiff = 1.;

    double sinbh = calendar->pullVariable("sinb");

    par.total = 0.5*irradiation*sinbh*(1. + 0.4*sinbh)/dsinbe;
    par.diffuse = sinbh*frdiff*atmtr*0.5*sc;
    if (par.diffuse > par.total)
        par.diffuse = par.total;
    par.direct = par.total - par.diffuse;
}

} //namespace

