/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    InputRef(double, sinb, "calendar[sinb]");
    InputRef(double, angot, "calendar[angot]");
    InputRef(double, irradiationCorrection, "calendar[irradiationCorrection]");
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
    records = seekOneChild<Model*>("records");
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
    updatePar();
}

void Weather::updatePar() {
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

    par.total = 0.5*irradiation*irradiationCorrection;
    par.diffuse = sinb*frdiff*atmtr*0.5*sc;
    if (par.diffuse > par.total)
        par.diffuse = par.total;
    par.direct = par.total - par.diffuse;
}

} //namespace

