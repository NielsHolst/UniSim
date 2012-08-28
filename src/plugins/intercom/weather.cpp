/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cfloat>
#include <usbase/clock.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/calendar.h"
#include "weather.h"

using namespace std;
using namespace UniSim;

namespace intercom{

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Variable<double>("Tavg", &Tavg, this,
    "Daily average temperature calculated as the average of @F Tmin and @F Tmax ({@Degree}C)");
    new Variable<double>("Tday", &Tday, this,
    "Daily day temperature calculated as @F Tmax minus one quarter of the span "
    "between @F Tmax and @F Tmin ({@Degree}C)");
    new Variable<double>("irradiation", &irradiation, this,
    "Daily total irradiation. Same as @F irradiationMJ read from weather records "
    "but with adjusted units (W/m @Sup{2})");
    new Variable<double>("parTotal", &par.total, this,
    "Total PAR irradiation rate at current time of day (W/m @Sup{2})");
    new Variable<double>("parDiffuse", &par.diffuse, this,
    "Diffuse component of total PAR irradiation at current time of day (W/m @Sup{2})");
    new Variable<double>("parDirect", &par.direct, this,
    "Direct componente of total PAR irradiation at current time of day (W/m @Sup{2})");
}

void Weather::initialize() {
    calendar = seekOne<Calendar*>("calendar");
    records = seekOneChild<Model*>("records");

    connect(clock(), SIGNAL(tick(double)), this, SLOT(handleClockTick(double)));
}

void Weather::reset() {
    update();
}

void Weather::update() {
    double Tmin = records->pullValue<double>("Tmin");
    double Tmax = records->pullValue<double>("Tmax");
    Tavg = (Tmin + Tmax)/2.;
    Tday = Tmax - 0.25*(Tmax - Tmin);
    irradiation = records->pullValue<double>("irradiationMJ")*1e6;
}

void Weather::handleClockTick(double hour) {
    updatePar();
}

void Weather::updatePar() {
    double
        sinld = calendar->pullValue<double>("sinLD"),
        cosld = calendar->pullValue<double>("cosLD"),
        day = calendar->pullValue<int>("dayOfYear"),
        dayLength = calendar->pullValue<double>("dayLength");

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

    double sinbh = calendar->pullValue<double>("sinb");

    par.total = 0.5*irradiation*sinbh*(1. + 0.4*sinbh)/dsinbe;
    par.diffuse = sinbh*frdiff*atmtr*0.5*sc;
    if (par.diffuse > par.total)
        par.diffuse = par.total;
    par.direct = par.total - par.diffuse;
}

} //namespace

/*
void Weather::verifySequence() {
    Deleted to avoid dependence on usengine

    Simulation *simulation = seekOneAscendant<Simulation*>("*");
    const Models &sequence(simulation->sequence());
    int ixCalendar(-1), ixWeather(-1);
    for (int i = 0; i < sequence.size(); ++i) {
        if (sequence.at(i) == calendar) ixCalendar = i;
        if (sequence.at(i) == this) ixWeather = i;
    }
    Q_ASSERT(ixCalendar > -1 && ixWeather > -1);
    if (ixCalendar > ixWeather)
        throw Exception("Calendar must appear before weather in simulation sequence. "
                        "Reorder 'sequence' elements in XML file.");
}
*/
