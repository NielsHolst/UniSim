/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cover_dew.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
CoverDew::CoverDew(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addParameterRef<int>(Name(numTimeSteps), "calendar[totalTimeSteps]");
    addParameterRef<double>(Name(coverArea), "greenhouse/construction[coverArea]");
    addParameterRef<double>(Name(groundArea), "greenhouse/construction[groundArea]");
    addParameterRef<double>(Name(Tcover), "../temperature[value]");
    addParameterRef<double>(Name(mcIndoors), "climate/humidity[mc]");
    addParameterRef<double>(Name(dewScreen), "regulation/screens[dew]");
    addVariable<double>(Name(value), "Dew formation on greenhouse cover (g/m2/s)");
}

void CoverDew::reset() {
    value = 0.;
}

void CoverDew::update() {
    const int oneDay = 24*60*60;
    if (timeStepSecs*numTimeSteps < oneDay) {
        value = 0.;
    }
    else {
        const double gcnd = 3e-3; //  m/s
        double smc = saturatedMoistureContent(Tcover);
        double value1 = (mcIndoors < smc) ? 0. :
            gcnd*(mcIndoors - smc)*coverArea/groundArea;
        value = (value1 < dewScreen) ? 0. : value1 - dewScreen;
    }
}

} //namespace

