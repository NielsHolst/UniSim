/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cfloat>
#include <usbase/clock.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "../unisim_models/calendar.h"
#include "weather.h"

using namespace std;
using namespace UniSim;

namespace intercom{

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new PullVariable<double>("Tavg", &Tavg, this, "description");
    new PullVariable<double>("Tday", &Tday, this, "description");
    new PullVariable<double>("irradiation", &irradiation, this, "description");
    new PullVariable<double>("parTotal", &par.total, this, "description");
    new PullVariable<double>("parDiffuse", &par.diffuse, this, "description");
    new PullVariable<double>("parDirect", &par.direct, this, "description");
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
    double Tmin = records->pullVariable<double>("Tmin");
    double Tmax = records->pullVariable<double>("Tmax");
    Tavg = (Tmin + Tmax)/2.;
    Tday = Tmax - 0.25*(Tmax - Tmin);
    irradiation = records->pullVariable<double>("irradiationMJ")*1e6;
}

void Weather::handleClockTick(double hour) {
    updatePar();
}

void Weather::updatePar() {
    double
        sinld = calendar->pullVariable<double>("sinLD"),
        cosld = calendar->pullVariable<double>("cosLD"),
        day = calendar->pullVariable<double>("dayOfYear"),
        dayLength = calendar->pullVariable<double>("dayLength");

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

    double sinbh = calendar->pullVariable<double>("sinb");

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
