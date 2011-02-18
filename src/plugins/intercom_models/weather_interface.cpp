/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "weather_interface.h"

using namespace UniSim;

namespace intercom{

WeatherInterface::WeatherInterface(UniSim::Identifier name, QObject *parent)
    : WeatherFile(name, parent)
{
    new PullVariable<double>("Tavg", &Tavg, this, "description");
    new PullVariable<double>("Tday", &Tday, this, "description");
    new PullVariable<double>("irradiation", &irradiation, this, "description");
    new PullVariable<double>("parTotal", &par.total, this, "description");
    new PullVariable<double>("parDiffuse", &par.diffuse, this, "description");
    new PullVariable<double>("parDirect", &par.direct, this, "description");
}

} //namespace

